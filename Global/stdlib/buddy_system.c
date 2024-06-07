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

#define MAP_START (((char *)get_heap_start()) + MEM_SIZE)
#define MAP_END (((char *)get_heap_start()) + HEAP_SIZE)
#define MEM_START ((char *)get_heap_start())
#define MEM_END (((char *)get_heap_start()) + MEM_SIZE)

#include "stdint.h"
#include <stdio.h>

typedef enum states
{
    EMPTY = 0,
    SPLIT,
    FULL,
    ALLOCATED
} states;

/*
char *mem_start;
char *mem_end;
char *map_start;
char *map_end;

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
*/
void *heap_start;
void malloc_init(void *start, size_t_m size)
{
    heap_start = start;

    // Initialize binary tree
    for (char *i = MAP_START; i < MAP_END; i++)
        *((uint64_t *)i) = EMPTY;
}
void *get_heap_start()
{
    return heap_start;
}

void cascade_state(char *x, size_t_m i, states s);

size_t_m round_to_power_of_two(size_t_m s)
{
    if (!s)
        return 0;
    int i = 1;
    while ((i << 1) <= s)
        i <<= 1;
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

states read_state(char *x, size_t_m i)
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
    case ALLOCATED:
        aux = read_state(x, GET_PARENT(i));
        if (aux == EMPTY)
            set_state(x, GET_PARENT(i), SPLIT);
        else if (aux == SPLIT && (read_state(x, GET_BROTHER(i)) == FULL || read_state(x, GET_BROTHER(i)) == ALLOCATED))
            set_state(x, GET_PARENT(i), FULL);
        break;
    default:
        break;
    }
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

/**
 * @brief returns the size in bytes of the memory allocation represented by the node located at this index in the memory map
 */
size_t_m mem_size(size_t_m index)
{
    return (MEM_SIZE) >> height(index);
}

size_t_m map_index_to_mem_index(size_t_m index)
{
    size_t_m result = 0;
    size_t_m jump = mem_size(index);
    while (index > 0)
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
    size_t_m jump = (MEM_SIZE) >> 2;
    size_t_m map_index = 0;
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

size_t_m find_buddy(size_t_m size, size_t_m index, size_t_m current_size);

void *malloc_m(size_t_m size)
{
    if (size > MEM_SIZE)
        return NULL;
    size_t_m index = find_buddy(size, 0, MEM_SIZE);
    if (index == -1)
    {
        return NULL;
    }
    return (void *)(MEM_START + map_index_to_mem_index(index));
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
        if (read_state(MAP_START, index) == EMPTY)
        {
            set_state(MAP_START, index, ALLOCATED);
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
    size_t_m left = find_buddy(size, GET_LEFT(index), current_size / 2);
    if (left == -1)
    {
        return find_buddy(size, GET_RIGHT(index), current_size / 2);
    }
    return left;
}

void *realloc_m(void *ptr, size_t_m size)
{
    free_m(ptr);
    return malloc_m(size);
}

void free_m(void *ptr)
{
    if (((char *)ptr) >= MEM_START && ((char *)ptr) <= MEM_END)
        set_state(MAP_START, mem_index_to_map_index(((char *)ptr) - MEM_START), EMPTY);
}
/*
void print_m_rec(size_t_m i, size_t_m height, char avoid_empty);

void print_m(char avoid_empty)
{
    printf("Printing memory-map tree\n");
    printf("-----------------------------------------------------\n");
    print_m_rec(0, 0, avoid_empty);
    printf("-----------------------------------------------------\n");
    printf("Printing map as binary\n");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < MAP_END - MAP_START; i++)
    {
        printf("%d", IS_BIT_SET(((char *)MAP_START), i));
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
           (uint64_t)MAP_START, (uint64_t)MAP_END, (uint64_t)(MAP_END - MAP_START), (uint64_t)MEM_START, (uint64_t)MEM_END, (uint64_t)(MEM_SIZE));
}

void print_m_rec(size_t_m i, size_t_m height, char avoid_empty)
{
    if (i < 0 || i > (MAP_END - MAP_START) * 2)
        return;

    states s = read_state(MAP_START, i);
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
    else if (s == FULL)
        printf("F\n");
    else
        printf("A\n");

    print_m_rec(GET_RIGHT(i), height + 1, avoid_empty);
}
*/

// Damn, this project works
// It is slightly beautiful
// I am slightly proud
