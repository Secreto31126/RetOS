#include "proc.h"

bool children_death(pid_t pid)
{
    Process *p = get_process(pid);

    for (size_t i = 0; i < MAX_PROCESS_CHILDREN; i++)
    {
        if (p->children[i] >= 0)
        {
            return false;
        }
    }

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
    return ((size_t)p->condition_data) <= get_tick();
}

bool no_condition(pid_t pid)
{
    return true;
}
