#include "malloc.h"
// I have decided that the correct implementation for a buddy system's memory map is a binary tree
// Of course, since a binary tree is traditionally just a two-tailed list there are two options
// Use a naïve malloc to manage this binary tree

// Or implement as
// An array binary tree
// Like barbarian

// The latter is a
// Haiku. I will do that now
// Enjoy this journey

#define HEAD_SIZE 2

#define IS_BIT_SET(x, i) (((x)[(i) >> 3] & (1 << ((i) & 7))) != 0)
#define SET_BIT(x, i) (x)[(i) >> 3] |= (1 << ((i) & 7))
#define CLEAR_BIT(x, i) (x)[(i) >> 3] &= (1 << ((i) & 7)) ^ 0xFF

#define IS_LEFT(i) ((i) % 2)
#define GET_LEFT(i) (((i) << 1) + 1)
#define GET_RIGHT(i) (((i) << 1) + 2)
#define GET_PARENT(i) ((i - 1) >> 1)
#define GET_BROTHER(i) (IS_LEFT(i) ? GET_RIGHT(GET_PARENT(i)) : GET_LEFT(GET_PARENT(i)))

#include "stdint.h"
#include <stdio.h>

typedef enum states
{
    EMPTY = 0,
    SPLIT,
    FULL
} states;

char *mem_start;
char *mem_end;
char *map_start;
char *map_end;

void cascade_state(char *x, size_t_m i, states s);

size_t_m round_to_power_of_two(size_t_m s)
{
    if (!s)
        return 0;
    int i = 1;
    while ((i >> 1) <= s)
        i >>= 1;
    return i;
}

void set_state(char *x, size_t_m i, states s)
{
    size_t_m bit_index = (i << 1);
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
    default:
        break;
    }
    // 0 index is root node, does not cascade
    if (i)
        cascade_state(x, i, s);
}

states read_state(char *x, size_t_m i)
{
    if (IS_BIT_SET(x, i << 1))
    {
        return FULL;
    }
    if (IS_BIT_SET(x, (i << 1) + 1))
    {
        return SPLIT;
    }
    return EMPTY;
}

void cascade_state(char *x, size_t_m i, states s)
{
    states aux;
    switch (s)
    {
    case EMPTY:
        aux = read_state(x, GET_PARENT(i));
        if (aux == FULL)
            set_state(x, GET_PARENT(i), SPLIT);
        else if (aux == SPLIT && read_state(x, GET_BROTHER(i)) == EMPTY)
            set_state(x, GET_PARENT(i), EMPTY);
        break;
    case SPLIT:
        if (read_state(x, GET_PARENT(i)) != SPLIT)
            set_state(x, GET_PARENT(i), SPLIT);
        break;
    case FULL:
        aux = read_state(x, GET_PARENT(i));
        if (aux == EMPTY)
            set_state(x, GET_PARENT(i), SPLIT);
        else if (aux == SPLIT && read_state(x, GET_BROTHER(i)) == FULL)
            set_state(x, GET_PARENT(i), FULL);
        break;
    default:
        break;
    }
}

void set_heap(void *start, size_t_m size)
{
    // A complete binary tree has (2*LEAVES)-1 nodes
    // Unit represents the number of MEMORY_BLOCK-NODE pairs that can fit into the given heap
    size_t_m unit = size / (BLOCK + HEAD_SIZE * 2 - 1);

    mem_start = (char *)start;
    mem_end = mem_start + round_to_power_of_two(BLOCK * unit);

    map_start = mem_end;
    // The binary tree must be complete to be properly mapped in an array
    map_end = map_start + round_to_power_of_two((HEAD_SIZE * 2 - 1) * unit);

    // Initialize binary tree
    for (char *i = map_start; i < map_end; i++)
        *((uint64_t *)i) = EMPTY;
}

size_t_m height(size_t_m index)
{
    size_t_m ans = 0;
    while (index)
    {
        ans++;
        index >>= 1;
    }
    return ans;
}
size_t_m mem_size(size_t_m index)
{
    return (mem_end - mem_start) >> height(index);
}

// TODO actually implement these correctly
size_t_m map_index_to_mem_index(size_t_m index)
{
    size_t_m result = 0;
    size_t_m jump = mem_size(index);
    while (index)
    {
        if (!IS_LEFT(index))
            result += jump;
        index = GET_PARENT(index);
        jump <<= 1;
    }
    return result;
}

size_t_m mem_index_to_map_index(size_t_m index)
{
    size_t_m probe = 0;
    size_t_m jump = (mem_end - mem_start) >> 1;
}

size_t_m find_buddy(size_t_m size, size_t_m index, size_t_m current_size);

void *malloc_m(size_t_m size)
{
    if (size > mem_end - mem_start)
        return NULL;
    size_t_m index = find_buddy(size, 0, mem_end - mem_start);
    if (index == -1)
    {
        return NULL;
    }
    return (void *)(mem_start + map_index_to_mem_index(index));
}

/**
 * @brief dfs algorithm, searches for a node that represents the smallest unit of memory that can fit the request
 */
size_t_m find_buddy(size_t_m size, size_t_m index, size_t_m current_size)
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
        if (read_state(map_start, index) == EMPTY)
        {
            set_state(map_start, index, FULL);
            return index;
        }
        return -1;
    }
    // If a node is full, the children are considered full as well
    if (read_state(map_start, index) == FULL)
    {
        return -1;
    }
    // dfs recursion: search left, if not found, return right search
    size_t_m left = find_buddy(size, GET_LEFT(index), current_size / 2);
    if (left == -1)
    {
        return find_buddy(size, GET_RIGHT(index), current_size / 2);
    }
    return left;
}
void *realloc_m(void *ptr, size_t_m size)
{
    return NULL;
}
void free_m(void *ptr)
{
    if (((char *)ptr) > mem_start && ((char *)ptr) < mem_end)
        set_state(mem_start, mem_index_to_map_index(((char *)ptr) - mem_start), EMPTY);
}

void print_m_rec(size_t_m i, size_t_m height, char avoid_empty);

void print_m(char avoid_empty)
{
    printf("Printing memory-map tree\n");
    printf("-----------------------------------------------------\n");
    print_m_rec(0, 0, avoid_empty);
    printf("-----------------------------------------------------\n");
    printf("Printing map as binary\n");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < map_end - map_start; i++)
    {
        printf("%d", IS_BIT_SET(((char *)map_start), i));
        if (i % 2)
            printf(" ");
    }
    printf("\n");
    printf("-----------------------------------------------------\n");
}

void print_m_data()
{
    printf("Map start:%lu\n\
Map end:%lu\n\
Dif:%lu\n\
Mem start:%lu\n\
Mem end:%lu\n\
Dif:%lu\n",
           (uint64_t)map_start, (uint64_t)map_end, (uint64_t)(map_end - map_start), (uint64_t)mem_start, (uint64_t)mem_end, (uint64_t)(mem_end - mem_start));
}

void print_m_rec(size_t_m i, size_t_m height, char avoid_empty)
{
    if (i < 0 || i > (map_end - map_start) * 2)
        return;

    states s = read_state(map_start, i);
    // avoid empty branches
    if (s == EMPTY && avoid_empty)
        return;

    print_m_rec(GET_LEFT(i), height + 1, avoid_empty);
    for (int i = 0; i < height; i++)
        printf("  ");

    if (s == EMPTY)
        printf("E\n");
    else if (s == SPLIT)
        printf("S\n");
    else
        printf("F\n");

    print_m_rec(GET_RIGHT(i), height + 1, avoid_empty);
}

// Damn, this project works
// It is slightly beautiful
// I am slightly proud
