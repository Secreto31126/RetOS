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

void *create_process_idle()
{
    void *new_stack = malloc(IDLE_STACK_SIZE);

    if (!new_stack)
    {
        return NULL;
    }

    processes[0] = (Process){
        .pid = 0,
        .ppid = 0,
        .stack = new_stack,
        .running_stack = new_stack,
        .stack_size = IDLE_STACK_SIZE,
        .running_stack_size = IDLE_STACK_SIZE,
        .rsp = STACK_END(new_stack, IDLE_STACK_SIZE),
        .state = PROCESS_RUNNING,
        .next_child = NULL,
        .next_brother = NULL,
        .exit_code = 0,
        .priority = 1,
        .next_blocked = NULL,
        .block_condition = no_condition,
        .condition_data = {},
        .files = {0, 1, 2, 3},
    };

    for (size_t i = 4; i < MAX_PROCESS_FILES; i++)
    {
        processes[0].files[i] = -1;
    }

    pid = 0;
    active_processes_count = 1;
    every_processes_count = 0;

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
    if (active_processes_count + 1 > MAX_PROCESSES)
    {
        return -1;
    }

    bool tried_to_clean_dead_bodies = false;
FIND_PID:
    pid_t new_pid = every_processes_count % (MAX_PROCESSES - 1) + 1;
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

        return -1;
    }

    // It's me, hi, I'm the process it's me
    Process *parent = get_current_process();

    // If the parent has a bigger stack, use it
    size_t new_stack_size = parent->stack_size > MIN_STACK_SIZE ? parent->stack_size : MIN_STACK_SIZE;

    void *new_stack = malloc(new_stack_size);

    if (!new_stack)
    {
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
    processes[new_pid].next_child = NULL;
    processes[new_pid].next_brother = parent->next_child;
    parent->next_child = get_process(new_pid);
    processes[new_pid].exit_code = 0;
    processes[new_pid].priority = parent->priority;
    processes[new_pid].next_blocked = NULL;
    processes[new_pid].block_condition = no_condition;
    // Not worth looping, there's no biggie if it's trash
    // processes[new_pid].condition_data = {};
    for (size_t i = 0; i < MAX_PROCESS_FILES; i++)
    {
        if (IS_PIPE(parent->files[i]))
        {
            add_pipe_end(parent->files[i]);
        }

        processes[new_pid].files[i] = parent->files[i];
    }

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

    // Keep track of the process population
    active_processes_count++;
    every_processes_count++;
    robin_add(new_pid);

    return new_pid;
}

void set_exit_code(int status)
{
    get_current_process()->exit_code = status;
}

int kill_process(pid_t pid)
{
    if (pid == 0)
    {
        return 1;
    }

    if (pid == 1)
    {
        power_off();
    }

    Process *man_im_dead = get_process(pid);

    if (man_im_dead->state == NOT_THE_PROCESS_YOU_ARE_LOOKING_FOR || man_im_dead->state == PROCESS_DEAD || man_im_dead->state == PROCESS_ZOMBIE)
    {
        // ENOENT
        return 2;
    }

    // If the dead body is in its parents' house
    if (man_im_dead->running_stack != man_im_dead->stack)
    {
        memcpy(
            man_im_dead->running_stack,
            STACK_END(man_im_dead->stack, man_im_dead->stack_size) - man_im_dead->running_stack_size,
            man_im_dead->running_stack_size);
    }

    for (size_t i = 0; i < MAX_PROCESS_FILES; i++)
    {
        if (man_im_dead->files[i] != -1)
        {
            close(man_im_dead->files[i]);
        }
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

    Process *p = man_im_dead->next_child;
    while (p)
    {
        p->ppid = 1;

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

        if (!p->next_brother)
        {
            Process *init = get_process(1);
            p->next_brother = init->next_child;
            init->next_child = man_im_dead->next_child;
            break;
        }

        p = p->next_brother;
    }

    if (!free_stack && !stack_inherited)
    {
        ncPrint("Probably a memory leak just happened\n");
        // Why, you may ask? If the stack is the running of any of its children,
        // But none of them inherited it, then no one will ever try to delete it.
        // I can think of a few ways to fix it, but none of them are likely to
        // be programmed by me right now.
    }

    if (free_stack)
    {
        free(man_im_dead->stack);
    }

    if (free_running_stack)
    {
        free(man_im_dead->running_stack);
    }

    man_im_dead->state = PROCESS_ZOMBIE;
    active_processes_count--;

    // ncPrint("Process ");
    // ncPrintDec(pid);
    // ncPrint(" killed\n");

    return 0;
}

int open_file(int file, int flags)
{
    Process *p = get_current_process();

    for (size_t i = 0; i < MAX_PROCESS_FILES; i++)
    {
        if (p->files[i] == -1)
        {
            p->files[i] = file | flags;
            return i;
        }
    }

    return -1;
}

int close(int fd)
{
    Process *p = get_current_process();

    if (0 <= fd && fd < MAX_PROCESS_FILES && p->files[fd] != -1)
    {
        if (IS_PIPE(p->files[fd]))
        {
            close_pipe(p->files[fd]);
        }

        p->files[fd] = -1;
        return 0;
    }

    return -1;
}

int dup2(int oldfd, int newfd)
{
    Process *p = get_current_process();

    if (oldfd == newfd)
    {
        return newfd;
    }

    if (0 <= oldfd && oldfd < MAX_PROCESS_FILES && 0 <= newfd && newfd < MAX_PROCESS_FILES)
    {
        if (p->files[oldfd] != -1)
        {
            close(p->files[newfd]);
        }

        p->files[newfd] = p->files[oldfd];
        return newfd;
    }

    return -1;
}
