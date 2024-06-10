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
        .name = "idle",
        .stack = new_stack,
        .running_stack = new_stack,
        .stack_size = IDLE_STACK_SIZE,
        .running_stack_size = IDLE_STACK_SIZE,
        .rsp = STACK_END(new_stack, IDLE_STACK_SIZE),
        .state = PROCESS_RUNNING,
        .next_child = NULL,
        .next_brother = NULL,
        .exit_code = 0,
        .priority = 19,
        .block_list = NULL,
        .next_blocked = NULL,
        .block_condition = no_condition,
        .condition_data = {},
        .zombie_sem = {},
        .exit_sem = {},
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

pid_t getpid()
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

    if (sem_init(&processes[new_pid].exit_sem, 1, 0))
    {
        free(new_stack);
        return -1;
    }

    if (sem_init(&processes[new_pid].zombie_sem, 1, 0))
    {
        sem_destroy(&processes[new_pid].exit_sem);
        free(new_stack);
        return -1;
    }

    // Set the new process' basic properties
    processes[new_pid].pid = new_pid;
    processes[new_pid].ppid = parent->pid;
    processes[new_pid].name = parent->name;
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
    processes[new_pid].block_list = NULL;
    processes[new_pid].next_blocked = NULL;
    processes[new_pid].block_condition = no_condition;
    // Not worth looping, there's no biggie if it's trash
    // processes[new_pid].condition_data = {};
    for (size_t i = 0; i < MAX_PROCESS_FILES; i++)
    {
        if (parent->files[i] != -1 && IS_PIPE(parent->files[i]))
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
    if (man_im_dead->state == PROCESS_RUNNING && man_im_dead->running_stack != man_im_dead->stack)
    {
        memcpy(
            man_im_dead->running_stack,
            STACK_END(man_im_dead->stack, man_im_dead->stack_size) - man_im_dead->running_stack_size,
            man_im_dead->running_stack_size);
    }

    if (man_im_dead->state == PROCESS_BLOCKED)
    {
        man_im_dead->block_condition = no_condition;
        Process **head = man_im_dead->block_list;
        *head = loop_blocked_and_unblock(*head);
    }

    sem_destroy(&man_im_dead->zombie_sem);

    for (size_t i = 0; i < MAX_PROCESS_FILES; i++)
    {
        close_file(man_im_dead->files[i]);
        man_im_dead->files[i] = -1;
    }

    Process *init = get_process(1);

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
        p->ppid = init->pid;
        if (p->state == PROCESS_ZOMBIE)
        {
            sem_post(&init->zombie_sem);
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
            // If a process still needs the running stack to run (pseudo pagination), don't free it
            free_running_stack = false;
        }

        if (!p->next_brother)
        {
            p->next_brother = init->next_child;
            init->next_child = man_im_dead->next_child;
            break;
        }

        p = p->next_brother;
    }

    size_t count = active_processes_count;
    for (size_t i = 0; i < MAX_PROCESS_FILES && count && (free_stack || free_running_stack); i++)
    {
        Process *p = get_process(i);

        if (p->pid != man_im_dead->pid)
        {
            if (p->running_stack == man_im_dead->running_stack)
            {
                free_running_stack = false;
            }

            if (!stack_inherited && p->running_stack == man_im_dead->stack)
            {
                free_stack = false;
                stack_inherited = inherit_parents_house(p);
            }
        }

        if (p->running_stack)
        {
            count--;
        }
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

    sem_post(&man_im_dead->exit_sem);
    sem_post(&get_process(man_im_dead->ppid)->zombie_sem);

    man_im_dead->state = PROCESS_ZOMBIE;
    man_im_dead->running_stack = NULL;
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

int close_file(int file)
{
    if (file != -1)
    {
        // You can't close stdin, stdout, stderr or stdkey
        if (0 <= file && file < 4)
        {
            return 0;
        }

        if (IS_PIPE(file))
        {
            return close_pipe(file);
        }
    }

    return -1;
}

int close(int fd)
{
    Process *p = get_current_process();

    if (0 <= fd && fd < MAX_PROCESS_FILES && !close_file(p->files[fd]))
    {
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
        close(newfd);

        p->files[newfd] = p->files[oldfd];
        if (IS_PIPE(p->files[newfd]))
        {
            add_pipe_end(p->files[newfd]);
        }

        return newfd;
    }

    return -1;
}

int getpriority(int which, id_t who)
{
    if (which == PRIO_PROCESS)
    {
        Process *p = get_process(who);
        return p->priority;
    }

    return -1;
}

int setpriority(int which, id_t who, int prio)
{
    if (which == PRIO_PROCESS)
    {
        Process *p = get_process(who);
        p->priority = MAX(PRIO_MIN, MIN(PRIO_MAX, prio));
        return 0;
    }

    return -1;
}

void *sbrk(intptr_t increment)
{
    Process *p = get_current_process();

    if (increment == 0)
    {
        return p->running_stack;
    }

    return (void *)-1;
}

int ps()
{
    __label__ exit;

    int pipesfd[2];
    if (pipe(pipesfd) == -1)
    {
        return -1;
    }

    ssize_t written;
    size_t expected;
#define pipe_write(x)                                       \
    written = write(pipesfd[1], (x), expected = strlen(x)); \
    if (written < 0 || (size_t)written < expected)          \
        goto exit;

    pipe_write("PID\tPPID\tSTATE\tPRIO\t\tSTACK\tCOMMAND\n");

    char buffer[1024];
    size_t remaining = active_processes_count;
    for (size_t i = 0; i < MAX_PROCESSES && remaining; i++)
    {
        Process *p = get_process(i);

        if (p->state == NOT_THE_PROCESS_YOU_ARE_LOOKING_FOR || p->state == PROCESS_DEAD)
        {
            continue;
        }

        utoa(p->pid, buffer, 10);
        pipe_write(buffer);
        pipe_write("\t");

        utoa(p->ppid, buffer, 10);
        pipe_write(buffer);
        pipe_write("\t\t");

        char state[2] = {p->state, 0};
        pipe_write(state);
        pipe_write("\t\t");

        itoa(p->priority, buffer, 10);
        pipe_write(buffer);
        pipe_write("\t\t");

        ultoa((unsigned long)p->stack, buffer, 16);
        pipe_write(buffer);
        pipe_write("\t");

        pipe_write(p->name);

        pipe_write("\n");
        remaining--;
    }

exit:
#undef pipe_write

    close(pipesfd[1]);
    return pipesfd[0];
}
