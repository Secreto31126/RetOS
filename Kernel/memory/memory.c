#include "memory.h"

void init_memory_manager()
{
    for (map_entry *map = map_start; map < heap_start; map++)
    {
        *map = 0;
    }
}
