#include "proc.h"

int kill(pid_t pid, int sig)
{
    if (pid < 2)
    {
        return -1;
    }

    Process *p = get_process(pid);

    if (p->state == NOT_THE_PROCESS_YOU_ARE_LOOKING_FOR ||
        p->state == PROCESS_DEAD ||
        p->state == PROCESS_ZOMBIE)
    {
        return -1;
    }

    switch (sig)
    {
    case SIGKILL:
        p->exit_code = 127;
        if (kill_process(p->pid))
        {
            return -1;
        }
        break;

    case SIGCONT:
        p->block_condition = no_condition;
        Process **head = p->block_list;
        *head = loop_blocked_and_unblock(*head);
        p->condition_data[0] = (void *)-1;
        break;

    case SIGSTOP:
        add_tick_blocked(p, whim_condition, NULL, NULL, NULL, NULL, NULL);
        break;

    default:
        return -1;
    }

    sched_yield();
    return 0;
}
