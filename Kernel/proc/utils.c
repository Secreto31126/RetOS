#include "proc.h"

bool inherit_parents_house(Process *process)
{
    // Should never happen
    if (process->running_stack == process->stack)
    {
        return false;
    }

    // Too small to live in there
    if (process->running_stack_size < process->stack_size)
    {
        return false;
    }

    memcpy(
        STACK_END(process->running_stack, process->running_stack_size) - process->stack_size,
        process->stack,
        process->stack_size);

    // The stack can be freed without checking other process because:
    // - If the process didn't have a child, the stack isn't being used by any other process
    // - If the process had a child before execv, it will be using the original running stack, not the parent's stack
    // - If the process had a child after execv, the process wouldn't have different stack and running stack
    free(process->stack);

    process->stack = process->running_stack;
    process->stack_size = process->running_stack_size;

    return true;
}

/**
 * @brief Reset a process to its own stack
 * @note This function should only be called from execv in assembler
 * (which should be called from a syscall for "atomicity")
 *
 * @param process The process to expell from its parent's house
 */
void move_away_from_parents_house(Process *process)
{
    if (process->running_stack != process->stack)
    {
        swap_stacks(
            process->running_stack,
            STACK_END(process->stack, process->stack_size) - process->running_stack_size,
            process->running_stack_size);

        process->running_stack = process->stack;
        process->running_stack_size = process->stack_size;
    }
}
