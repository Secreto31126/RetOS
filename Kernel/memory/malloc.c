#include "memory.h"

void *malloc(uint16_t size)
{
    // Hardcoded limit to 32kb per call
    if (!size || size & 0x8000)
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

        map_entry *start = map;
        while (!*map)
        {
            if (map - start + 1 < size)
            {
                map += sizeof(map_entry);
                continue;
            }

            *(start + map_size) = size * sizeof(map_entry);
            return start + map_size;
        }
    }

    return NULL;
}
