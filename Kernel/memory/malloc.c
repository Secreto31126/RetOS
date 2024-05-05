#include "memory.h"

#define MEM_SCALE 1
void *malloc(uint32_t size)
{
    if (!size || size > heap_size)
    {
        return NULL;
    }

    map_entry *map = map_start;
    while (map < map_end)
    {
        if (*map)
        {
            map += *map;
            continue;
        }

        for (uint64_t i = 1; map + i < map_end; i++)
        {
            if (i * MEM_SCALE * sizeof(map_entry) >= size)
            {
                *map = i;
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
