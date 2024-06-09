#include "proc.h"

bool multi_read_available(pid_t pid)
{
    Process *p = get_process(pid);
    int nfds = (uintptr_t)p->condition_data[0];
    int *fds = p->condition_data[1];
    int *ready = p->condition_data[2];

    size_t ready_count = 0;
    bool all_closed = true;

    for (size_t i = 0; i < nfds; i++)
    {
        int fd = fds[i];

        if (fd < 0 || fd >= MAX_PROCESS_FILES)
        {
            continue;
        }

        int file = p->files[fd];

        if (file == -1)
        {
            continue;
        }

        all_closed = false;

        if (!file_empty(file))
        {
            ready[ready_count++] = fd;
        }
    }

    if (ready_count || all_closed)
    {
        p->condition_data[0] = NULL + ready_count;
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
