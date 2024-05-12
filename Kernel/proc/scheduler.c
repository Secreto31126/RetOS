#include "proc.h"
#include <console.h>

/**
 * @brief Handle a context switch request, preserving the current rsp and returning the next rsp
 *
 * @note This function should only be called from assembly within a tick interruption
 * and interruptions disabled, hence not declared in proc.h
 *
 * @param rsp The current rsp
 * @return void* The next rsp
 */
void *context_switch(void *rsp)
{
    Process *old_process = get_current_process();

    // Only validate pseudo stackoverflow protection if the process is alive
    if (old_process->state != PROCESS_DEAD && old_process->state != PROCESS_ZOMBIE && old_process->state != NOT_THE_PROCESS_YOU_ARE_LOOKING_FOR)
    {
        if (rsp < old_process->running_stack || STACK_END(old_process->running_stack, old_process->running_stack_size) < rsp)
        {
            ncPrint("Possible Stack Overflow for PID ");
            ncPrintDec(old_process->pid);
            ncPrint(" (RSP: ");
            ncPrintHex((uint64_t)rsp);
            ncPrint(")\n");
        }
    }

    if (old_process->state == PROCESS_RUNNING)
    {
        old_process->rsp = rsp;
        old_process->state = PROCESS_READY;
    }
    else if (old_process->state == PROCESS_BLOCKED)
    {
        old_process->rsp = rsp;
        robin_remove(old_process->pid);
    }

    check_blocked_processes();
    set_pid(robin_next());

    Process *new_process = get_current_process();

    // If the process changed
    if (old_process->pid != new_process->pid)
    {
        // If the old process isn't (living) dead
        if (old_process->state != PROCESS_DEAD && old_process->state != PROCESS_ZOMBIE)
        {
            // If the kid is in its parents' house
            if (old_process->running_stack != old_process->stack)
            {
                swap_stacks(
                    old_process->running_stack,
                    STACK_END(old_process->stack, old_process->stack_size) - old_process->running_stack_size,
                    old_process->running_stack_size);
            }
        }

        // If the new process still lives with its parents
        if (new_process->stack != new_process->running_stack)
        {
            swap_stacks(
                STACK_END(new_process->stack, new_process->stack_size) - new_process->running_stack_size,
                new_process->running_stack,
                new_process->running_stack_size);
        }
    }

    new_process->state = PROCESS_RUNNING;
    return new_process->rsp;
}

// Priority based round Robin
typedef struct p_node
{
    pid_t pid;
    struct p_node *next;
} p_node;

p_node *first = NULL;
p_node *last = NULL;
signed char remaining = 0;

void robin_add(pid_t pid)
{
    p_node *to_add = malloc(sizeof(p_node));

    to_add->pid = pid;
    to_add->next = NULL;

    if (first == NULL)
    {
        first = to_add;
        remaining = get_process(first->pid)->priority;
    }
    else
    {
        last->next = to_add;
    }

    last = to_add;
}

static p_node *remove_rec_p(pid_t pid, p_node *node)
{
    if (node == NULL)
    {
        return node;
    }

    if (node->pid == pid)
    {
        p_node *next = node->next;
        free(node);

        return next;
    }

    node->next = remove_rec_p(pid, node->next);
    if (node->next == NULL)
    {
        last = node;
    }

    return node;
}

void robin_remove(pid_t pid)
{
    if (first == NULL)
    {
        return;
    }

    if (first->pid == pid)
    {
        p_node *aux = first;
        first = first->next;
        free(aux);

        if (first == NULL)
        {
            last = NULL;
            return;
        }

        remaining = get_process(first->pid)->priority;
        return;
    }

    first->next = remove_rec_p(pid, first->next);
    if (first->next == NULL)
    {
        last = first;
    }

    return;
}

pid_t robin_next()
{
    if (first == NULL)
    {
        return 0;
    }

    Process *p = get_process(first->pid);
    if (p->state == PROCESS_DEAD || p->state == PROCESS_ZOMBIE)
    {
        robin_remove(first->pid);
        return robin_next();
    }

    if (remaining-- > 0)
    {
        return first->pid;
    }

    if (first->next == NULL)
    {
        remaining = p->priority;
        return first->pid;
    }

    p_node *aux = first;
    first = first->next;

    last->next = aux;
    aux->next = NULL;

    last = aux;

    remaining = get_process(first->pid)->priority;
    return first->pid;
}

void yield_robin()
{
    remaining = 0;
}
