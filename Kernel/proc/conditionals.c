#include "proc.h"

bool zombie_child(pid_t pid)
{
    Process *p = get_process(pid);

    // Shouldn't happen
    if (p->next_child == NULL)
    {
        p->condition_data[0] = (void *)-1;
        return true;
    }

    pid_t looking_for = (uintptr_t)p->condition_data[0];

    // If looking for a specific process and it's not a zombie, O(1)
    if (looking_for != -1)
    {
        Process *child = get_process(looking_for);

        if (child->state != PROCESS_ZOMBIE)
        {
            return false;
        }
    }
    // If it is a zombie, O(n) (we must search the previous child to unlink it from the list)

    Process *prev = NULL;
    Process *child = p->next_child;
    while (looking_for != child->pid && !(looking_for == -1 && child->state == PROCESS_ZOMBIE))
    {
        prev = child;
        child = child->next_brother;

        if (!child)
        {
            return false;
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

    int *wstatus = (int *)p->condition_data[1];

    if (wstatus)
    {
        *wstatus = child->exit_code;
    }

    child->state = PROCESS_DEAD;
    p->condition_data[0] = NULL + child->pid;

    return true;
}

bool semaphore_raised(pid_t pid)
{
    // Process *p = get_process(pid);

    // Semaphore logic here

    return true;
}

bool read_available(pid_t pid)
{
    // Process *p = get_process(pid);

    // Read lock logic here

    return true;
}

bool sleep_finished(pid_t pid)
{
    Process *p = get_process(pid);
    if (((size_t)p->condition_data[0]) <= get_tick())
    {
        p->condition_data[0] = 0;
        return true;
    }

    return false;
}

bool no_condition(pid_t pid)
{
    return true;
}
