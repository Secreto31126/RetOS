#include "proc.h"
#include <console.h>

/**
 * @brief Handle a context switch request, preserving the current rsp and returning the next rsp
 *
 * @note This function should only be called from assembly within a tick interruption
 * and interruptions disabled, hence not declared in proc.h
 *
 * @param rsp The current rsp
 * @return void* The next rsp
 */
void *context_switch(void *rsp)
{
    Process *old_process = get_current_process();

    // Only validate pseudo stackoverflow protection if the process is alive
    if (old_process->state != PROCESS_DEAD && old_process->state != PROCESS_ZOMBIE && old_process->state != NOT_THE_PROCESS_YOU_ARE_LOOKING_FOR)
    {
        if (rsp < old_process->running_stack || STACK_END(old_process->running_stack, old_process->running_stack_size) < rsp)
        {
            ncPrint("Possible Stack Overflow for PID ");
            ncPrintDec(old_process->pid);
            ncPrint(" (RSP: ");
            ncPrintHex((uint64_t)rsp);
            ncPrint(")\n");
        }
    }

    if (old_process->state == PROCESS_RUNNING)
    {
        old_process->rsp = rsp;
        old_process->state = PROCESS_READY;
    }
    else if (old_process->state == PROCESS_BLOCKED)
    {
        old_process->rsp = rsp;
    }

    check_tick_blocked_processes();
    set_pid(robin_next());

    Process *new_process = get_current_process();

    // If the process changed
    if (old_process->pid != new_process->pid)
    {
        // If the old process isn't (living) dead
        if (old_process->state != PROCESS_DEAD && old_process->state != PROCESS_ZOMBIE)
        {
            // If the kid is in its parents' house
            if (old_process->running_stack != old_process->stack)
            {
                swap_stacks(
                    old_process->running_stack,
                    STACK_END(old_process->stack, old_process->stack_size) - old_process->running_stack_size,
                    old_process->running_stack_size);
            }
        }

        // If the new process still lives with its parents
        if (new_process->stack != new_process->running_stack)
        {
            swap_stacks(
                STACK_END(new_process->stack, new_process->stack_size) - new_process->running_stack_size,
                new_process->running_stack,
                new_process->running_stack_size);
        }
    }

    // ncPrintDec(old_process->pid);
    // ncPrint(" -> ");
    // ncPrintDec(new_process->pid);
    // ncNewline();

    new_process->state = PROCESS_RUNNING;
    return new_process->rsp;
}

// Priority based round Robin
// =========================================================================================================================================================
typedef struct pNode
{
    struct pNode *next;
    pid_t pid;
} pNode;
typedef struct iterableListHeader
{
    pNode *first;
    pNode *current;
    int size;
} iterableListHeader;
typedef iterableListHeader *iterableList;

iterableList get_il()
{
    iterableList to_ret = malloc(sizeof(iterableListHeader));
    if (to_ret == NULL)
        return NULL;
    to_ret->first = NULL;
    to_ret->current = NULL;
    to_ret->size = 0;
    return to_ret;
}
pid_t next_il(iterableList il)
{
    if (!il->size || il->first == NULL)
        return 0;
    if (il->current == NULL)
        il->current = il->first;
    pid_t to_ret = il->current->pid;
    il->current = il->current->next;
    return to_ret;
}
int add_il(iterableList il, pid_t pid)
{
    pNode *to_add = malloc(sizeof(pNode));
    if (to_add == NULL)
        return 1;
    to_add->next = il->first;
    to_add->pid = pid;

    il->first = to_add;
    il->size++;
    return 0;
}
pid_t remove_il_rec(pNode *p, pNode *current, pid_t pid);
pid_t remove_il(iterableList il, pid_t pid)
{
    if (!il->size || il->first == NULL)
        return 0;
    int to_ret = remove_il_rec(il->first, il->current, pid);
    if (to_ret)
        il->size--;
    return to_ret;
}
pid_t remove_il_rec(pNode *p, pNode *current, pid_t pid)
{
    if (p->next == NULL)
        return 0;
    if (p->next->pid == pid)
    {
        pNode *to_free = p->next;
        if (current == p->next)
            current = p->next->next;
        p->next = p->next->next;
        free(to_free);
        return pid;
    }
    return remove_il_rec(p->next, current, pid);
}

static const char weights[PRIO_MAX - PRIO_MIN] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39};
static int schedule[PRIO_MAX - PRIO_MIN] = {0};
static int schedule_index = 0;
static int schedule_remaining = 0;
static int ready_count = 0;
static iterableList process_lists[PRIO_MAX - PRIO_MIN] = {0};

int get_weight(int priority)
{
    return weights[priority - PRIO_MIN];
}
iterableList get_proc_list_entry(int index)
{
    iterableList il = process_lists[index - PRIO_MIN];
    return il ? il : NULL;
}
void set_proc_list_entry(int index, iterableList entry)
{
    process_lists[index - PRIO_MIN] = entry;
}
void set_schedule_entry(int index, int entry)
{
    schedule[index - PRIO_MIN] = entry;
}
int get_schedule_entry(int index)
{
    return schedule[index - PRIO_MIN];
}
void set_schedule()
{
    schedule_remaining = 0;
    schedule_index = 0;
    for (int i = PRIO_MIN; i < PRIO_MAX; i++)
    {
        iterableList il = get_proc_list_entry(i);
        if (get_il(i) == NULL || il->size <= 0)
        {
            set_schedule_entry(i, 0);
        }
        else
        {
            int entry = process_lists[i]->size * get_weight(i) / ready_count; // A simple equation to assign time to each priority based on weights
            if (entry <= il->size)                                            // Ensure that every process gets at least one turn in the schedule
                entry = il->size;
            schedule_remaining += entry; // Record how many rounds are scheduled
            set_schedule_entry(i, entry);
        }
    }
}

void robin_add(pid_t pid)
{
    Process *p = get_process(pid);
    int priority = p->priority;
    iterableList il = get_proc_list_entry(priority);
    if (!il || il == NULL)
    {
        il = get_il();
        if (il == NULL)
            return;
        set_proc_list_entry(priority, il);
    }
    if (add_il(il, pid))
        ready_count++;
}

pid_t robin_remove(pid_t pid)
{
    Process *p = get_process(pid);
    int priority = p->priority;
    // Look for it where you expect it to be first (In the list defined by its priority)
    if (remove_il(get_proc_list_entry(priority), pid))
    {
        ready_count--;
        return;
    }

    // If it is not found, it still could be in another list, as priorities can change without warning the scheduler
    for (int i = PRIO_MIN; i < PRIO_MAX; i++)
    {
        if (i != priority && remove_il(get_proc_list_entry(i), pid))
        {
            ready_count--;
            return pid;
        }
    }
    return 0;
}

int next_schedule()
{
    if (!schedule_remaining)
        return -1;
    schedule_index = (schedule_index + 1) % PRIO_MAX;
    int entry = get_schedule_entry(schedule_index);
    if (entry <= 0)
        return (next_schedule());
    set_schedule_entry(schedule_index, entry - 1);
    return schedule_index;
}

pid_t robin_next()
{
    if (!ready_count)
        return 0;
    if (!schedule_remaining)
        set_schedule();

    int scheduled_priority = next_schedule();
    if (scheduled_priority < 0)
        return 0;

    pid_t to_ret = next_il(get_proc_list_entry(scheduled_priority));
    if (!to_ret)
        return robin_next();
    Process *p_to_ret = get_process(to_ret);

    if (p_to_ret->state == PROCESS_DEAD || p_to_ret->state == PROCESS_ZOMBIE || p_to_ret->state == PROCESS_BLOCKED)
    {
        robin_remove(to_ret);
        return robin_next();
    }

    if (p_to_ret->priority != scheduled_priority) // Its priority has changed, move it to the correct list
    {
        robin_remove(to_ret);
        robin_add(to_ret);
    }

    return to_ret;
}

void yield_robin()
{
    return; // There is no such thing here
}
