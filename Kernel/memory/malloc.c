#include "memory.h"

void *malloc(uint32_t size)
{
    if (!size || size > heap_size)
    {
        return NULL;
    }

    map_entry *map = map_start;
    while (map < heap_start)
    {
        if (*map)
        {
            map += *map;
            continue;
        }

        for (uint64_t i = 0; map + i * sizeof(map_entry) < heap_start && !map[i]; i++)
        {
            if (i * sizeof(map_entry) >= size)
            {
                *map_start = size * sizeof(map_entry);
                return map_start + map_size;
            }
        }
    }

    return NULL;
}
