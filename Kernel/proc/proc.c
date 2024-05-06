#include "proc.h"

/**
 * @brief The current process ID
 */
pid_t pid;

pid_t active_processes_count;
pid_t every_processes_count;

/**
 * @brief The array of all processes
 */
Process processes[MAX_PROCESSES] = {};

void *create_process_init()
{
    void *new_stack = malloc(INIT_STACK_SIZE);

    if (!new_stack)
    {
        return NULL;
    }

    processes[0] = (Process){
        .pid = 0,
        .ppid = 0,
        .stack = new_stack,
        .running_stack = new_stack,
        .stack_size = INIT_STACK_SIZE,
        .running_stack_size = INIT_STACK_SIZE,
        .rsp = STACK_END(new_stack, INIT_STACK_SIZE),
        .state = PROCESS_RUNNING,
    };

    pid = 0;
    active_processes_count = 1;
    every_processes_count = 1;

    ncPrintHex((uint64_t)processes[0].stack);

    return processes[0].rsp;
}

pid_t get_pid()
{
    return pid;
}

pid_t set_pid(pid_t p)
{
    pid_t o = pid;
    pid = p;
    return o;
}

Process *get_current_process()
{
    return get_process(pid);
}

Process *get_process(pid_t p)
{
    return processes + p;
}

pid_t create_process(void *rsp)
{
    // Pseudo semaphore
    unset_interrupt_flag();

    if (active_processes_count + 1 > MAX_PROCESSES)
    {
        set_interrupt_flag();
        return -1;
    }

    bool tried_to_clean_dead_bodies = false;
FIND_PID:
    pid_t new_pid = 1;
    while (new_pid < MAX_PROCESSES && processes[new_pid].state != NOT_THE_PROCESS_YOU_ARE_LOOKING_FOR)
    {
        new_pid++;
    }

    if (new_pid == MAX_PROCESSES)
    {
        if (!tried_to_clean_dead_bodies)
        {
            for (size_t i = 0; i < MAX_PROCESSES; i++)
            {
                Process *p = get_process(i);
                if (p->state == PROCESS_DEAD)
                {
                    p->state = NOT_THE_PROCESS_YOU_ARE_LOOKING_FOR;
                }
            }

            tried_to_clean_dead_bodies = true;
            goto FIND_PID;
        }

        set_interrupt_flag();
        return -1;
    }

    // It's me, hi, I'm the process it's me
    Process *parent = get_current_process();

    if (parent->children_count + 1 > MAX_PROCESS_CHILDREN)
    {
        set_interrupt_flag();
        return -1;
    }

    // If the parent has a bigger stack, use it
    size_t new_stack_size = parent->stack_size > MIN_STACK_SIZE ? parent->stack_size : MIN_STACK_SIZE;

    void *new_stack = malloc(new_stack_size);

    if (!new_stack)
    {
        set_interrupt_flag();
        return -1;
    }

    // Set the new process' basic properties
    processes[new_pid].pid = new_pid;
    processes[new_pid].ppid = parent->pid;
    processes[new_pid].stack = new_stack;
    processes[new_pid].running_stack = parent->running_stack;
    processes[new_pid].stack_size = new_stack_size;
    processes[new_pid].running_stack_size = parent->running_stack_size;
    processes[new_pid].rsp = rsp;
    processes[new_pid].state = PROCESS_READY;
    for (size_t i = 0; i < MAX_PROCESSES; i++)
    {
        processes[new_pid].children[i] = -1;
    }
    processes[new_pid].children_count = 0;

    // Set the parent's children
    parent->children[parent->children_count] = new_pid;

    // Keep track of the process population
    parent->children_count++;
    active_processes_count++;
    every_processes_count++;

    // We aren't Linux, we must copy the stack at creation time
    memcpy(
        STACK_END(new_stack, new_stack_size) - parent->running_stack_size,
        parent->running_stack,
        parent->running_stack_size);

    // ncPrint("\n\tNew process ");
    // ncPrintDec(new_pid);
    // ncPrint(" created with rsp 0x");
    // ncPrintHex((uint64_t)rsp);
    // ncPrint(" and stack 0x");
    // ncPrintHex((uint64_t)new_stack);
    // ncPrint(" and stack_end 0x");
    // ncPrintHex((uint64_t)STACK_END(new_stack, new_stack_size));
    // ncNewline();

    set_interrupt_flag();

    return new_pid;
}

int kill_process(pid_t pid)
{
    // Pseudo semaphore
    unset_interrupt_flag();

    Process *man_im_dead = get_process(pid);

    if (man_im_dead->state == NOT_THE_PROCESS_YOU_ARE_LOOKING_FOR)
    {
        set_interrupt_flag();

        // ENOENT
        return 2;
    }

    /**
     * @brief Wether the stack should be freed or not (another process is using it as running stack)
     */
    bool free_stack = true;
    /**
     * @brief Wether another process moved into the stack (free_stack is set to false)
     */
    bool stack_inherited = false;
    /**
     * @brief Wether the running stack should be freed or not (another process is using it as running stack)
     */
    bool free_running_stack = man_im_dead->stack != man_im_dead->running_stack;

    for (pid_t i = 0; i < MAX_PROCESSES; i++)
    {
        if (i == pid)
        {
            continue;
        }

        Process *p = get_process(i);

        if (p->state == NOT_THE_PROCESS_YOU_ARE_LOOKING_FOR)
        {
            continue;
        }

        // Process inheritance
        if (p->ppid == pid)
        {
            p->ppid = man_im_dead->ppid;
        }

        // Stacks preservation (pseudo pagination)
        if (p->running_stack == man_im_dead->stack)
        {
            // If a process still needs the stack to run (pseudo pagination), don't free it
            free_stack = false;

            // Try to inherit the stack from the parent
            if (!stack_inherited)
            {
                stack_inherited = inherit_parents_house(p);
            }
        }

        if (p->running_stack == man_im_dead->running_stack)
        {
            // If another process is still using the running stack (pseudo pagination), don't free it
            free_running_stack = false;
        }
    }

    if (free_stack)
    {
        free(man_im_dead->stack);
    }

    if (free_running_stack)
    {
        free(man_im_dead->running_stack);
    }

    man_im_dead->state = PROCESS_DEAD;
    active_processes_count--;

    ncPrint("Process ");
    ncPrintDec(pid);
    ncPrint(" killed\n");

    set_interrupt_flag();

    return 0;
}
