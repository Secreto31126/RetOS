#include "mman.h"
#include <string.h>
#include <stdlib.h>

void init_memory_manager()
{
    for (map_entry *map = map_start; map < map_end; map++)
    {
        *map = 0;
    }
}

int memory_state(char *output, size_t length)
{
    char *start = output;
    *output = 0;

    map_entry *map = map_start;
    while (map < map_end)
    {
        if (!*map)
        {
            map++;
            continue;
        }

        if (output - start + 64 > length)
        {
            break;
        }

        void *location = (void *)map + map_size;
        int size = *map;

        strcpy(output, "Location: 0x");
        output += sizeof("Location: 0x") - 1;
        ultoa((uint64_t)location, output, 16);
        output += strlen(output);

        strcpy(output, "\tSize: 0x");
        output += sizeof("\tSize: 0x") - 1;
        ultoa(size * sizeof(map_entry), output, 16);
        output += strlen(output);

        strcpy(output, "\n");
        output++;

        map += *map;
    }

    return output - start;
}
