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
    Process *p = get_process(pid);
    int file = (uintptr_t)p->condition_data[0];
    return !file_empty(file);
}

bool write_available(pid_t pid)
{
    Process *p = get_process(pid);
    int file = (uintptr_t)p->condition_data[0];
    return !file_full(file);
}

bool multi_read_available(pid_t pid)
{
    Process *p = get_process(pid);
    int nfds = (uintptr_t)p->condition_data[0];
    int *fds = p->condition_data[1];
    int *ready = p->condition_data[2];

    size_t i = 0;
    bool all_closed = true;

    for (i = 0; i < nfds; i++)
    {
        int file = p->files[*fds];

        if (file == -1)
        {
            continue;
        }

        all_closed = false;

        if (!file_empty(file))
        {
            ready[i++] = *fds;
        }
    }

    if (i || all_closed)
    {
        p->condition_data[0] = NULL + i;
        return true;
    }

    return false;
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

bool custom_condition(pid_t pid)
{
    return get_process(pid)->condition_data[0];
}

bool no_condition(pid_t pid)
{
    return true;
}

bool whim_condition(pid_t pid)
{
    return false;
}
