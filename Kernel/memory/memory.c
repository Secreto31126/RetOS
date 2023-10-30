#include "memory.h"

void init_memory_manager()
{
    for (uint64_t *map = map_start; map < heap_start; map++)
    {
        *map = 0;
    }
}
