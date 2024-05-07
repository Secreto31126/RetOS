#include "mman.h"

void init_memory_manager()
{
    for (map_entry *map = map_start; map < map_end; map++)
    {
        *map = 0;
    }
}
