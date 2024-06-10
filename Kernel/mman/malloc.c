#include "mman.h"

#ifdef BUDDY_SYSTEM

void *malloc(uint32_t size)
{
    if (size > MEM_END - MEM_START)
        return NULL;
    size_t index = find_buddy(size * 2, 0, MEM_END - MEM_START);
    if (index == -1)
    {
        return NULL;
    }
    return (void *)(MEM_START + map_index_to_mem_index(index));
}

#else

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

#endif
