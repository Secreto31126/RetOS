#include "proc.h"

static Process *loop_blocked_and_unblock(Process *p)
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
    p->next_blocked = NULL;
    robin_add(p->pid);

    return loop_blocked_and_unblock(next);
}

void check_blocked_processes()
{
    Process *idle = get_process(0);
    idle->next_blocked = loop_blocked_and_unblock(idle->next_blocked);
}

void add_blocked(Process *p, ProcessBlockConditional condition, void *data0, void *data1, void *data2, void *data3, void *data4)
{
    p->block_condition = condition;
    p->condition_data[0] = data0;
    p->condition_data[1] = data1;
    p->condition_data[2] = data2;
    p->condition_data[3] = data3;
    p->condition_data[4] = data4;

    if (p->state != PROCESS_BLOCKED)
    {
        Process *idle = get_process(0);

        p->next_blocked = idle->next_blocked;
        idle->next_blocked = p;

        p->state = PROCESS_BLOCKED;
    }
}

pid_t waitpid(pid_t pid, int *wstatus, int options)
{
    Process *p = get_current_process();

    if (pid != -1)
    {
        Process *child = get_process(pid);

        if (child->ppid != p->pid)
        {
            return -1;
        }
    }

    int *wstatus_ptr = wstatus ? malloc(sizeof(int)) : NULL;

    add_blocked(p, zombie_child, NULL + (pid), wstatus_ptr, NULL, NULL, NULL);
    sched_yield();

    if (wstatus)
    {
        *wstatus = *wstatus_ptr;
    }

    free(wstatus_ptr);

    return (uintptr_t)p->condition_data[0];
}

unsigned int sleep(unsigned int seconds)
{
    Process *p = get_current_process();
    add_blocked(p, sleep_finished, NULL + (get_tick() + seconds * 18), NULL, NULL, NULL, NULL);
    sched_yield();
    return (uintptr_t)p->condition_data[0];
}

unsigned int usleep(unsigned int usec)
{
    Process *p = get_current_process();
    add_blocked(p, sleep_finished, NULL + (get_tick() + usec), NULL, NULL, NULL, NULL);
    sched_yield();
    return (uintptr_t)p->condition_data[0] ? -1 : 0;
}

void read_block(int file)
{
    Process *p = get_current_process();
    add_blocked(p, read_available, NULL + file, NULL, NULL, NULL, NULL);
    sched_yield();
}

void write_block(int file)
{
    Process *p = get_current_process();
    add_blocked(p, write_available, NULL + file, NULL, NULL, NULL, NULL);
    sched_yield();
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

    add_blocked(p, multi_read_available, NULL + nfds, copy_fds, ready_ptr, NULL, NULL);
    sched_yield();

    int ready_count = (uintptr_t)p->condition_data[0];
    memcpy(ready, ready_ptr, ready_count * sizeof(int));

    free(copy_fds);
    free(ready_ptr);

    return ready_count;
}

void sem_block(sem_t *sem)
{
    Process *p = get_current_process();
    add_blocked(p, semaphore_raised, sem, NULL, NULL, NULL, NULL);
    sched_yield();
}
