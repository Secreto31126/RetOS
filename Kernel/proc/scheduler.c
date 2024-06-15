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
typedef struct p_node
{
    struct p_node *next;
    pid_t pid;
} p_node;
typedef struct iterable_listHeader
{
    p_node *first;
    p_node *current;
    int size;
} iterable_listHeader;
typedef iterable_listHeader *iterable_list;

iterable_list get_il()
{
    iterable_list to_ret = malloc(sizeof(iterable_listHeader));
    if (to_ret == NULL)
        return NULL;
    to_ret->first = NULL;
    to_ret->current = NULL;
    to_ret->size = 0;
    return to_ret;
}
pid_t next_il(iterable_list il)
{
    if (il == NULL || !il->size || il->first == NULL)
        return 0;
    if (il->current == NULL)
        il->current = il->first;
    pid_t to_ret = il->current->pid;
    il->current = il->current->next;
    return to_ret;
}
int add_il(iterable_list il, pid_t pid)
{
    p_node *to_add = malloc(sizeof(p_node));
    if (to_add == NULL)
        return 1;
    to_add->next = il->first;
    to_add->pid = pid;

    il->first = to_add;
    il->size++;
    return 0;
}
pid_t remove_il_rec(p_node *p, iterable_list il, pid_t pid);
pid_t remove_il(iterable_list il, pid_t pid)
{
    if (il == NULL || !il->size || il->first == NULL)
        return 0;
    if (il->first->pid == pid)
    {
        p_node *to_free = il->first;
        if (il->current == il->first)
            il->current = il->first->next;
        il->first = il->first->next;
        il->size--;
        free(to_free);
        return pid;
    }
    int to_ret = remove_il_rec(il->first, il, pid);
    if (to_ret)
        il->size--;
    return to_ret;
}
pid_t remove_il_rec(p_node *p, iterable_list il, pid_t pid)
{
    if (p->next == NULL)
        return 0;
    if (p->next->pid == pid)
    {
        p_node *to_free = p->next;
        if (il->current == p->next)
            il->current = il->first;
        p->next = p->next->next;
        free(to_free);
        return pid;
    }
    return remove_il_rec(p->next, il, pid);
}

static unsigned int seed = 0x50;
static unsigned int bit;
unsigned int rand()
{ // A linear-feedback shift register ('sourced' from wikipedia)
    bit = ((seed >> 0) ^ (seed >> 2) ^ (seed >> 3) ^ (seed >> 5)) & 1;
    return (seed = (seed >> 1) | (bit << 15));
}
unsigned int rand_between(unsigned int min, unsigned int max)
{
    if (min >= max)
        return 0;
    return (rand() % (max - min)) + min;
}

static const char weights[PRIO_MAX - PRIO_MIN + 1] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
static const int weights_sum = 820;
static int ready_count = 0;
static iterable_list process_lists[PRIO_MAX - PRIO_MIN + 1] = {0};

int get_weight(int priority)
{
    return weights[priority - PRIO_MIN];
}
iterable_list get_proc_list_entry(int index)
{
    iterable_list il = process_lists[index - PRIO_MIN];
    return il ? il : NULL;
}
void set_proc_list_entry(int index, iterable_list entry)
{
    process_lists[index - PRIO_MIN] = entry;
}

char is_valid_entry(iterable_list il)
{
    return il != NULL && il->first != NULL && il->size > 0;
}

static iterable_list current_priority = NULL;
static int current_priority_index = 0;
static int remaining = 0;
iterable_list next_schedule(int *priority)
{
    if (remaining > 0 && is_valid_entry(current_priority))
    {
        remaining--;
        *priority = current_priority_index;
        return current_priority;
    }
    remaining = 0;

    int index = rand_between(0, weights_sum), i;
    iterable_list last_valid = NULL;
    for (i = PRIO_MAX; i >= PRIO_MIN; i--)
    {
        iterable_list aux = get_proc_list_entry(i);
        if (is_valid_entry(aux))
        {
            *priority = i;
            last_valid = aux;
        }
        index -= get_weight(i);
        if (index <= 0)
            break;
    }
    if (last_valid != NULL)
    {
        remaining = last_valid->size - 1;
        current_priority = last_valid;
        current_priority_index = *priority; // I set it myself, may as well use it
        return last_valid;
    }
    for (; i >= PRIO_MIN; i--)
    {
        iterable_list aux = get_proc_list_entry(i);
        if (is_valid_entry(aux))
        {
            *priority = i;
            current_priority_index = i;
            current_priority = aux;
            remaining = aux->size;
            return aux;
        }
    }
    *priority = -1;
    return NULL;
}

void robin_add(pid_t pid)
{
    Process *p = get_process(pid);
    int priority = p->priority;
    iterable_list il = get_proc_list_entry(priority);
    if (il == NULL)
    {
        il = get_il();
        if (il == NULL)
            return;
        set_proc_list_entry(priority, il);
    }
    if (!add_il(il, pid))
    {
        ready_count++;
    }
}

pid_t robin_remove(pid_t pid)
{
    Process *p = get_process(pid);
    int priority = p->priority;
    // Look for it where you expect it to be first (in the list given by its priority)

    if (remove_il(get_proc_list_entry(priority), pid))
    {
        ready_count--;
        return pid;
    }
    // If it is not found, it still could be in another list, as priorities can change without warning the scheduler
    for (int i = PRIO_MIN; i <= PRIO_MAX; i++)
    {
        if (i != priority && remove_il(get_proc_list_entry(i), pid))
        {
            ready_count--;
            return pid;
        }
    }
    return 0;
}

pid_t robin_next()
{
    if (!ready_count)
        return 0;

    int priority;
    iterable_list scheduled_priority = next_schedule(&priority);
    if (scheduled_priority == NULL)
        return 0;

    pid_t to_ret = next_il(scheduled_priority);

    if (to_ret <= 0)
    {
        return 0;
    }

    Process *p_to_ret = get_process(to_ret);
    if (p_to_ret->state == PROCESS_DEAD || p_to_ret->state == PROCESS_ZOMBIE || p_to_ret->state == PROCESS_BLOCKED)
    {
        robin_remove(to_ret);
        return robin_next();
    }

    if (p_to_ret->priority != priority) // Its priority has changed, move it to the correct list
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
