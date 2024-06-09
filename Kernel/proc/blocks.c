#include "proc.h"

Process *loop_blocked_and_unblock(Process *p)
{
    if (p == NULL)
    {
        return p;
    }

    Process *next = p->next_blocked;

    // If for some reason the process is not blocked,
    // Remove it silently from the blocked list
    if (p->state != PROCESS_BLOCKED)
    {
        p->block_condition = no_condition;
        p->next_blocked = NULL;
        return loop_blocked_and_unblock(next);
    }

    if (!(p->block_condition(p->pid)))
    {
        p->next_blocked = loop_blocked_and_unblock(next);
        return p;
    }

    p->state = PROCESS_READY;
    p->block_condition = no_condition;
    p->block_list = NULL;
    p->next_blocked = NULL;
    robin_add(p->pid);

    return loop_blocked_and_unblock(next);
}

void check_tick_blocked_processes()
{
    Process *idle = get_process(0);
    idle->next_blocked = loop_blocked_and_unblock(idle->next_blocked);
}

void add_tick_blocked(Process *p, ProcessBlockConditional condition, void *data0, void *data1, void *data2, void *data3, void *data4)
{
    if (p->state == PROCESS_BLOCKED)
    {
        p->block_condition = no_condition;
        Process **head = p->block_list;
        *head = loop_blocked_and_unblock(*head);
    }

    Process *idle = get_process(0);

    p->block_list = &idle->next_blocked;
    p->next_blocked = idle->next_blocked;
    idle->next_blocked = p;
    p->block_condition = condition;
    p->condition_data[0] = data0;
    p->condition_data[1] = data1;
    p->condition_data[2] = data2;
    p->condition_data[3] = data3;
    p->condition_data[4] = data4;
    p->state = PROCESS_BLOCKED;
}

/**
 * @brief Wait any pid
 *
 * @param wstatus The exit status of the process
 * @param options The wait options
 * @return pid_t The pid of the process that exited, -1 if error
 */
static pid_t waitapid(int *wstatus, int options)
{
    Process *p = get_current_process();

    if (sem_wait(&p->zombie_sem))
    {
        return -1;
    }

    // Shouldn't happen
    if (!p->next_child)
    {
        return -1;
    }

    Process *prev = NULL;
    Process *child = p->next_child;
    while (child->state != PROCESS_ZOMBIE)
    {
        prev = child;
        child = child->next_brother;

        if (!child)
        {
            return -1;
        }
    }

    if (prev)
    {
        prev->next_brother = child->next_brother;
    }
    else
    {
        p->next_child = child->next_brother;
    }

    if (wstatus)
    {
        *wstatus = child->exit_code;
    }

    sem_destroy(&child->exit_sem);
    child->state = PROCESS_DEAD;
    child->ppid = 0;

    return child->pid;
}

pid_t waitpid(pid_t pid, int *wstatus, int options)
{
    Process *p = get_current_process();

    if (!p->next_child)
    {
        return -1;
    }

    if (pid == -1)
    {
        return waitapid(wstatus, options);
    }

    Process *child = get_process(pid);

    if (child->ppid != p->pid)
    {
        return -1;
    }

    if (sem_wait(&child->exit_sem))
    {
        return -1;
    }

    // We must search the previous child to unlink it from the list (O(n))
    Process *prev = NULL;
    Process *child = p->next_child;
    while (pid != child->pid && !(pid == -1 && child->state == PROCESS_ZOMBIE))
    {
        prev = child;
        child = child->next_brother;

        if (!child)
        {
            return -1;
        }
    }

    if (prev)
    {
        prev->next_brother = child->next_brother;
    }
    else
    {
        p->next_child = child->next_brother;
    }

    if (wstatus)
    {
        *wstatus = child->exit_code;
    }

    // Discard the dead process from the zombie's semaphore
    sem_wait(&p->zombie_sem);
    sem_destroy(&child->exit_sem);
    child->state = PROCESS_DEAD;
    child->ppid = 0;

    return child->pid;
}

unsigned int sleep(unsigned int seconds)
{
    Process *p = get_current_process();
    add_tick_blocked(p, sleep_finished, NULL + (get_tick() + seconds * 18), NULL, NULL, NULL, NULL);
    sched_yield();
    return (uintptr_t)p->condition_data[0];
}

unsigned int usleep(unsigned int usec)
{
    Process *p = get_current_process();
    add_tick_blocked(p, sleep_finished, NULL + (get_tick() + usec), NULL, NULL, NULL, NULL);
    sched_yield();
    return (uintptr_t)p->condition_data[0] ? -1 : 0;
}

int pselect(int nfds, const int *fds, int *ready)
{
    Process *p = get_current_process();

    int *copy_fds = malloc(nfds * sizeof(int));

    if (!copy_fds)
    {
        return -1;
    }

    memcpy(copy_fds, fds, nfds * sizeof(int));

    int *ready_ptr = malloc(nfds * sizeof(int));

    if (!ready_ptr)
    {
        free(copy_fds);
        return -1;
    }

    add_tick_blocked(p, multi_read_available, NULL + nfds, copy_fds, ready_ptr, NULL, NULL);
    sched_yield();

    int ready_count = (uintptr_t)p->condition_data[0];

    if (ready_count > 0)
    {
        memcpy(ready, ready_ptr, ready_count * sizeof(int));
    }

    free(copy_fds);
    free(ready_ptr);

    return ready_count;
}
