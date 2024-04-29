#include "proc.h"

void exit()
{
    Process *process = get_current_process();
    free(process->stack);
    process->stack = NULL;
    process->rsp = NULL;
    process->pid = -1;
    processes_count--;
}
