#include "proc.h"

#define STACK_END(stack, size) (((void *)(stack)) + (size))

pid_t pid = 0;

pid_t processes_count = 1;
Process processes[MAX_PROCESSES] = {
    {
        .pid = 0,
        .stack = (void *)0x50000,
        // Kernel has no concept of stack limit or top,
        // it believes all the RAM belongs to it.
        // This property is meaningless to kernel,
        // we just pray it doesn't use more than this.
        .stack_size = 0x400,
        .rsp = (void *)0x50400, // The initial rsp setted by Pure64
        .state = PROCESS_RUNNING,
    },
};

pid_t get_pid()
{
    return pid;
}

Process *get_current_process()
{
    return processes + pid;
}

pid_t create_process(void *rsp, void **i_rsp)
{
    // Pseudo semaphore
    unset_interrupt_flag();

    if (processes_count + 1 >= MAX_PROCESSES)
    {
        return -1;
    }

    pid_t new_pid = processes_count++;

    set_interrupt_flag();

    Process *parent = get_current_process();

    size_t new_stack_size = parent->stack_size < MIN_STACK_SIZE ? MIN_STACK_SIZE : parent->stack_size;

    void *new_stack = malloc(new_stack_size);

    if (!new_stack)
    {
        return -1;
    }

    // Set the new process' basic properties
    processes[new_pid].pid = new_pid;
    processes[new_pid].stack = new_stack;
    processes[new_pid].stack_size = parent->stack_size;
    processes[new_pid].state = PROCESS_READY;

    // We aren't Linux, we must copy the stack at creation time
    memcpy(STACK_END(new_stack, new_stack_size) - parent->stack_size, parent->stack, parent->stack_size);

    processes[new_pid].rsp = STACK_END(new_stack, new_stack_size) - (STACK_END(parent->stack, parent->stack_size) - rsp);

    void **new_i_rsp = STACK_END(parent->stack, parent->stack_size) - ((void *)i_rsp);
    *new_i_rsp = STACK_END(new_stack, new_stack_size) - (STACK_END(parent->stack, parent->stack_size) - *i_rsp);

    return new_pid;
}
