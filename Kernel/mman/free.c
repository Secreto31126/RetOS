#include "mman.h"

#ifdef BUDDY_SYSTEM

void free(void *ptr)
{
    if (((char *)ptr) >= mem_start && ((char *)ptr) <= mem_end)
        set_node_state(map_start, mem_index_to_map_index(((char *)ptr) - mem_start), EMPTY);
}

#else

void free(void *ptr)
{
    map_entry *pointer = (map_entry *)((uint64_t)ptr - heap_size);
    if (map_start <= pointer && pointer < map_start + map_size)
    {
        *pointer = 0;
    }
}

#endif
