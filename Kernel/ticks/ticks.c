#include <ticks.h>
#include <lib.h>

static uint64_t current_tick = 0;

typedef struct
{
    Callback task;
    uint64_t tick;
} Task;

#define MAX_TASKS 256
static uint8_t active_tasks = 0;
static Task task_list[MAX_TASKS] = {0};

uint64_t get_tick()
{
    return current_tick;
}

void update_tick()
{
    // Every second
    if (current_tick % 18)
    {
        update_header();
    }

    current_tick++;

    uint8_t counted = 0;
    for (uint8_t i = 0; counted < active_tasks && i < MAX_TASKS; i++)
    {
        if (!task_list[i].task)
            continue;

        if (task_list[i].tick == current_tick)
        {
            task_list[i].task();
            task_list[i].task == 0;
            active_tasks--;
        }
        else
        {
            counted++;
        }
    }
}

void sleep_ticks(uint64_t ticks)
{
    uint64_t end = current_tick + ticks;
    while (current_tick < end)
        halt_once();
}

void add_task(uint64_t ticks, Callback task)
{
    if (!task)
        return;

    if (!ticks)
        return task();

    if (active_tasks >= MAX_TASKS)
        return;

    uint8_t i = 0;
    while (task_list[i].task != 0)
        i++;

    task_list[i].task = task;
    task_list[i].tick = current_tick + ticks;

    active_tasks++;
}
