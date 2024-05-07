#include "mman.h"

void free(void *ptr)
{
    map_entry *pointer = (map_entry *)((uint64_t)ptr - heap_size);
    if (map_start <= pointer && pointer < map_start + map_size)
    {
        *pointer = 0;
    }
}
