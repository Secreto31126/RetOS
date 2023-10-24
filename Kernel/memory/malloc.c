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
            map += *map; // adds 32 * *map bytes
            continue;
        }

        for (uint64_t i = 1; map + i < heap_start; i++)
        {
            if (i * sizeof(map_entry) >= size)
            {
                *map = i * sizeof(map_entry);
                return (void *)map + map_size;
            }

            if (map[i])
            {
                map += i;
                break;
            }
        }
    }

    return NULL;
}
