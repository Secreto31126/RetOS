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
static Process *last = NULL;
static signed char remaining = 20;

void robin_add(pid_t pid)
{
    Process *p = get_process(pid);
    p->next_robin = NULL;

    Process *idle = get_process(0);

    if (idle->next_robin == NULL)
    {
        idle->next_robin = p;
        remaining = get_process(pid)->priority;
    }
    else
    {
        last->next_robin = p;
    }

    last = p;
}

static Process *remove_rec_p(pid_t pid, Process *p)
{
    if (p == NULL)
    {
        return p;
    }

    if (p->pid == pid)
    {
        Process *next = p->next_robin;
        p->next_robin = NULL;
        return next;
    }

    p->next_robin = remove_rec_p(pid, p->next_robin);
    if (p->next_robin == NULL)
    {
        last = p;
    }

    return p;
}

void robin_remove(pid_t pid)
{
    Process *idle = get_process(0);

    if (idle->next_robin == NULL)
    {
        return;
    }

    if (idle->next_robin->pid == pid)
    {
        Process *removed = idle->next_robin;
        idle->next_robin = removed->next_robin;
        removed->next_robin = NULL;

        if (idle->next_robin == NULL)
        {
            last = NULL;
            return;
        }

        remaining = idle->next_robin->priority;
        return;
    }

    idle->next_robin = remove_rec_p(pid, idle->next_robin);
    if (idle->next_robin->next_robin == NULL)
    {
        last = idle->next_robin;
    }

    return;
}

pid_t robin_next()
{
    Process *idle = get_process(0);
    Process *first = idle->next_robin;

    if (first == NULL)
    {
        return idle->pid;
    }

    if (first->state == PROCESS_DEAD || first->state == PROCESS_ZOMBIE)
    {
        robin_remove(first->pid);
        return robin_next();
    }

    if (remaining++ < 20)
    {
        return first->pid;
    }

    robin_remove(first->pid);
    robin_add(first->pid);

    return first->pid;
}

void yield_robin()
{
    remaining = PRIO_MAX + 1;
}
