#include "proc.h"

static Process *loop_blocked_and_unblock(Process *p)
{
    if (p == NULL)
    {
        return p;
    }

    Process *next = p->next_blocked;

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
    Process *idle = get_process(0);

    p->state = PROCESS_BLOCKED;
    p->block_condition = condition;
    p->condition_data[0] = data0;
    p->condition_data[1] = data1;
    p->condition_data[2] = data2;
    p->condition_data[3] = data3;
    p->condition_data[4] = data4;
    p->next_blocked = idle->next_blocked;
    idle->next_blocked = p;
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

    add_blocked(p, zombie_child, NULL + (pid), wstatus, NULL, NULL, NULL);
    yield();
    return (uintptr_t)p->condition_data[0];
}

unsigned int sleep(unsigned int seconds)
{
    Process *p = get_current_process();
    add_blocked(p, sleep_finished, NULL + (get_tick() + seconds * 18), NULL, NULL, NULL, NULL);
    yield();
    return (uintptr_t)p->condition_data[0];
}

unsigned int usleep(unsigned int usec)
{
    Process *p = get_current_process();
    add_blocked(p, sleep_finished, NULL + (get_tick() + usec), NULL, NULL, NULL, NULL);
    yield();
    return (uintptr_t)p->condition_data[0] ? -1 : 0;
}

void read_block(int file)
{
    Process *p = get_current_process();
    add_blocked(p, read_available, NULL + file, NULL, NULL, NULL, NULL);
    yield();
}

void write_block(int file)
{
    Process *p = get_current_process();
    add_blocked(p, write_available, NULL + file, NULL, NULL, NULL, NULL);
    yield();
}
