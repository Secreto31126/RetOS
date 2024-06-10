#include "mman.h"

#ifdef BUDDY_SYSTEM

size_t round_to_power_of_two(size_t s)
{
    if (!s)
        return 0;
    int i = 1;
    while ((i << 1) <= s)
        i <<= 1;
    return i;
}

void init_memory_manager()
{
    // Initialize binary tree
    for (char *i = MAP_START; i < MAP_END; i++)
    {
        *((uint64_t *)i) = EMPTY;
    }
}

void set_node_state(char *x, size_t i, states s)
{
    size_t bit_index = (i << 1);
    switch (s)
    {
    case EMPTY:
        CLEAR_BIT(x, bit_index);
        CLEAR_BIT(x, bit_index + 1);
        break;
    case SPLIT:
        CLEAR_BIT(x, bit_index);
        SET_BIT(x, bit_index + 1);
        break;
    case FULL:
        SET_BIT(x, bit_index);
        CLEAR_BIT(x, bit_index + 1);
        break;
    case ALLOCATED:
        SET_BIT(x, bit_index);
        SET_BIT(x, bit_index + 1);
        break;
    default:
        break;
    }
    // 0 index is root node, does not cascade
    if (i)
        cascade_state(x, i, s);
}

states read_state(char *x, size_t i)
{
    if (IS_BIT_SET(x, i << 1))
    {
        if (IS_BIT_SET(x, (i << 1) + 1))
            return ALLOCATED;
        return FULL;
    }
    if (IS_BIT_SET(x, (i << 1) + 1))
    {
        return SPLIT;
    }
    return EMPTY;
}

void cascade_state(char *x, size_t i, states s)
{
    states aux;
    switch (s)
    {
    case EMPTY:
        aux = read_state(x, GET_PARENT(i));
        if (aux == FULL)
            set_node_state(x, GET_PARENT(i), SPLIT);
        else if (aux == SPLIT && read_state(x, GET_BROTHER(i)) == EMPTY)
            set_node_state(x, GET_PARENT(i), EMPTY);
        break;
    case SPLIT:
        if (read_state(x, GET_PARENT(i)) != SPLIT)
            set_node_state(x, GET_PARENT(i), SPLIT);
        break;
    case FULL:
    case ALLOCATED:
        aux = read_state(x, GET_PARENT(i));
        if (aux == EMPTY)
            set_node_state(x, GET_PARENT(i), SPLIT);
        else if (aux == SPLIT && (read_state(x, GET_BROTHER(i)) == FULL || read_state(x, GET_BROTHER(i)) == ALLOCATED))
            set_node_state(x, GET_PARENT(i), FULL);
        break;
    default:
        break;
    }
}

size_t height(size_t index)
{
    size_t ans = 0;
    while (index)
    {
        ans++;
        index >>= 1;
    }

    return ans;
}

size_t mem_size(size_t index)
{
    return (MEM_END - MEM_START) >> height(index);
}

size_t map_index_to_mem_index(size_t index)
{
    size_t result = 0;
    size_t jump = mem_size(index);
    while (index > 0)
    {
        if (!IS_LEFT(index))
            result += jump;
        index = GET_PARENT(index);
        jump <<= 1;
    }
    return result;
}

size_t mem_index_to_map_index(size_t index)
{
    size_t probe = 0;
    size_t jump = (MEM_END - MEM_START) >> 2;
    size_t map_index = 0;
    while (probe < index && jump > 0)
    {
        if (probe + jump <= index)
        {
            probe += jump;
            map_index = GET_RIGHT(map_index);
        }
        else
        {
            map_index = GET_LEFT(map_index);
        }
        jump >>= 1;
    }

    if (!jump)
    {
        return map_index;
    }
    while (read_state(MAP_START, map_index) != ALLOCATED)
    {
        map_index = GET_LEFT(map_index);
    }
    return map_index;
}

size_t find_buddy(size_t size, size_t index, size_t current_size)
{
    // if does not fit in the space denoted by the node, do not look further
    if (size > current_size)
    {
        return -1;
    }
    // if does not fit in half the space of the node, you are in a smallest node that can fit it
    // If current_size is smaller than BLOCK, you have reached minimum allowed memory unit
    if (size > current_size / 2 || current_size / 2 <= BLOCK)
    {
        if (read_state(MAP_START, index) == EMPTY)
        {
            set_node_state(MAP_START, index, ALLOCATED);
            return index;
        }
        return -1;
    }
    // If a node is full, the children are considered full as well
    if (read_state(MAP_START, index) == FULL || read_state(MAP_START, index) == ALLOCATED)
    {
        return -1;
    }
    // dfs recursion: search left, if not found, return right search
    size_t left = find_buddy(size, GET_LEFT(index), current_size / 2);
    if (left == -1)
    {
        return find_buddy(size, GET_RIGHT(index), current_size / 2);
    }
    return left;
}

int memory_state(char *output, size_t length)
{
    return 1;
}

#else

void init_memory_manager()
{
    for (map_entry *map = MAP_START; map < MAP_END; map++)
    {
        *map = 0;
    }
}

int memory_state(char *output, size_t length)
{
    char *start = output;
    *output = 0;

    map_entry *map = MAP_START;
    while (map < MAP_END)
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

#endif
