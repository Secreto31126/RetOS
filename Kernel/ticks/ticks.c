#include <ticks.h>
#include <lib.h>

static uint64_t current_tick = 0;

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
}

void sleep_ticks(uint64_t ticks)
{
    uint64_t end = current_tick + ticks;
    while (current_tick < end)
        halt_once();
}
