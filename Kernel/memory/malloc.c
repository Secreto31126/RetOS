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

        map_entry *map_start = map;
        while (!*map)
        {
            if (map - map_start + sizeof(heap_entry) < size)
            {
                map += sizeof(map_entry);
                continue;
            }

            *map_start = size * sizeof(map_entry);
            return map_start + map_size;
        }
    }

    return NULL;
}
