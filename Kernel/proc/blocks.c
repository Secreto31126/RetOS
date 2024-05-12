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
    p->condition_data = NULL;
    p->next_blocked = NULL;
    robin_add(p->pid);

    return loop_blocked_and_unblock(next);
}

void check_blocked_processes()
{
    Process *init = get_process(0);
    init->next_blocked = loop_blocked_and_unblock(init->next_blocked);
}

void add_blocked(Process *p, ProcessBlockConditional condition, void *data)
{
    Process *init = get_process(0);

    p->state = PROCESS_BLOCKED;
    p->block_condition = condition;
    p->condition_data = data;
    p->next_blocked = init->next_blocked;
    init->next_blocked = p;
}

void waitpid()
{
    Process *p = get_current_process();
    add_blocked(p, children_death, NULL);
    yield();
}

void sleep(unsigned int seconds)
{
    Process *p = get_current_process();
    add_blocked(p, sleep_finished, (void *)(get_tick() + seconds * 18));
    yield();
}
