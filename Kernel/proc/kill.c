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
        kill_process(p->pid);
        return 0;

    case SIGCONT:
        p->block_condition = no_condition;
        Process **head = p->block_list;
        *head = loop_blocked_and_unblock(*head);
        p->condition_data[0] = -1;
        return 0;

    case SIGSTOP:
        add_tick_blocked(p, whim_condition, NULL, NULL, NULL, NULL, NULL);
        return 0;

    default:
        return -1;
    }
}
