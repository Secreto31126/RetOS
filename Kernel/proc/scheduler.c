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
    }

    check_tick_blocked_processes();
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

    // ncPrintDec(old_process->pid);
    // ncPrint(" -> ");
    // ncPrintDec(new_process->pid);
    // ncNewline();

    new_process->state = PROCESS_RUNNING;
    return new_process->rsp;
}

// Priority based round Robin
// =========================================================================================================================================================
typedef struct pNode
{
    struct pNode *next;
    pid_t pid;
} pNode;
typedef struct iterableListHeader
{
    pNode *first;
    pNode *current;
    int size;
} iterableListHeader;
typedef iterableListHeader *iterableList;

iterableList get_il()
{
    iterableList to_ret = malloc(sizeof(iterableListHeader));
    if (to_ret == NULL)
        return NULL;
    to_ret->first = NULL;
    to_ret->current = NULL;
    to_ret->size = 0;
    return to_ret;
}
pid_t next_il(iterableList il, int move)
{
    if (il == NULL || !il->size || il->first == NULL)
        return 0;
    if (il->current == NULL)
        il->current = il->first;
    pid_t to_ret = il->current->pid;
    if (move)
        il->current = il->current->next;
    return to_ret;
}
int add_il(iterableList il, pid_t pid)
{
    pNode *to_add = malloc(sizeof(pNode));
    if (to_add == NULL)
        return 1;
    to_add->next = il->first;
    to_add->pid = pid;

    il->first = to_add;
    il->size++;
    return 0;
}
pid_t remove_il_rec(pNode *p, iterableList il, pid_t pid);
pid_t remove_il(iterableList il, pid_t pid)
{
    if (il == NULL || !il->size || il->first == NULL)
        return 0;
    if (il->first->pid == pid)
    {
        pNode *to_free = il->first;
        il->first = il->first->next;
        il->size--;
        free(to_free);
        return pid;
    }
    int to_ret = remove_il_rec(il->first, il, pid);
    if (to_ret)
        il->size--;
    return to_ret;
}
pid_t remove_il_rec(pNode *p, iterableList il, pid_t pid)
{
    if (p->next == NULL)
        return 0;
    if (p->next->pid == pid)
    {
        pNode *to_free = p->next;
        if (il->current == p->next)
            il->current = il->first;
        p->next = p->next->next;
        free(to_free);
        return pid;
    }
    return remove_il_rec(p->next, il, pid);
}

static const char weights[PRIO_MAX - PRIO_MIN + 1] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
static int remaining = 0;
static iterableList process_list = NULL;

void robin_add(pid_t pid)
{
    Process *p = get_process(pid);
    if (process_list == NULL)
    {
        process_list = get_il();
        if (process_list == NULL)
            return;
    }
    add_il(process_list, pid);
}

pid_t robin_remove(pid_t pid)
{
    return remove_il(process_list, pid);
}

pid_t robin_next_aux()
{
    remaining--;
    if (remaining < 0)
    {
        pid_t to_ret = next_il(process_list, 0);
        remaining = get_process(to_ret)->priority - PRIO_MIN;
        return to_ret;
    }
    if (!remaining)
    {
        return next_il(process_list, 1);
    }
    return next_il(process_list, 0);
}

pid_t robin_next()
{
    pid_t to_ret = robin_next_aux();
    if (!to_ret)
        return to_ret;
    Process *p_to_ret = get_process(to_ret);

    if (p_to_ret->state == PROCESS_DEAD || p_to_ret->state == PROCESS_ZOMBIE || p_to_ret->state == PROCESS_BLOCKED)
    {
        robin_remove(to_ret);
        return robin_next();
    }
    if (to_ret)
    {
        ncPrintHex(to_ret);
        ncPrint(" ");
    }
    return to_ret;
}

void yield_robin()
{
    remaining = 0;
}
