#include "proc.h"
#include <console.h>

/**
 * @brief Handle a context switch request, preserving the current rsp and returning the next rsp
 *
 * @note This function should only be called from assembly with interruptions disabled, hence not declared in proc.h
 *
 * @param rsp The current rsp
 * @return void* The next rsp
 */
void *context_switch(void *rsp)
{
    Process *process;

    process = get_current_process();
    process->rsp = rsp;
    process->state = PROCESS_READY;

    // ncPrint("[PID ");
    ncTab();
    ncPrintDec(process->pid);
    // ncPrint("]\n");

    do
    {
        pid++;
        pid %= processes_count;

        // ncPrint("Status PID ");
        // ncPrintDec(pid);
        // ncPrint(": ");
        // ncPrint(get_current_process()->state == PROCESS_READY ? "READY (" : "NOT READY (");
        // ncPrintDec(get_current_process()->state);
        // ncPrint(")\n");

    } while (get_current_process()->state != PROCESS_READY); // Get the next ready process

    process = get_current_process();
    process->state = PROCESS_RUNNING;

    // ncPrint("[PID ");
    ncTab();
    ncPrintDec(process->pid);
    // ncPrint("]\n");

    return process->rsp;
}
