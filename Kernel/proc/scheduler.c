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

    if (old_process->state == PROCESS_RUNNING)
    {
        old_process->rsp = rsp;

        if (rsp < old_process->running_stack || STACK_END(old_process->running_stack, old_process->running_stack_size) < rsp)
        {
            ncPrint("Possible Stack Overflow for PID ");
            ncPrintDec(old_process->pid);
            ncPrint(" (RSP: ");
            ncPrintHex((uint64_t)rsp);
            ncPrint(")\n");
        }

        old_process->state = PROCESS_READY;
    }

    Process *new_process;
    do
    {
        // pid++;
        // pid %= processes_count;
        set_pid((get_pid() + 1) % MAX_PROCESSES);

        // ncPrint("\nStatus PID ");
        // ncPrintDec(pid);
        // ncPrint(" (");
        // ncPrintHex((uint64_t)processes[pid].stack);
        // ncPrint(")");
        // ncPrint(": ");
        // ncPrint(get_current_process()->state == PROCESS_READY ? "READY (" : "NOT READY (");
        // ncPrintDec(get_current_process()->state);
        // ncPrint(")\n");

    } while ((new_process = get_current_process())->state != PROCESS_READY); // Get the next ready process

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
