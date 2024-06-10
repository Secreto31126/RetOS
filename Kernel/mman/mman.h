#ifndef PMMAN_H
#define PMMAN_H

#include <mman.h>
#include <string.h>
#include <stdlib.h>

#define BUDDY_SYSTEM

typedef uint32_t map_entry;

// 384 MB
#define HEAP_START 0x18000000
// 128 MB
#define HEAP_SIZE 0x8000000

#ifdef BUDDY_SYSTEM

#define HEAD_SIZE 2
#define BLOCK 256

#define IS_BIT_SET(x, i) (((x)[(i) >> 3] & (1 << ((i) & 7))) != 0)
#define SET_BIT(x, i) (x)[(i) >> 3] |= (1 << ((i) & 7))
#define CLEAR_BIT(x, i) (x)[(i) >> 3] &= (1 << ((i) & 7)) ^ 0xFF

#define IS_LEFT(i) ((i) % 2)
#define GET_LEFT(i) (((i) << 1) + 1)
#define GET_RIGHT(i) (((i) << 1) + 2)
#define GET_PARENT(i) ((i - 1) >> 1)
#define GET_BROTHER(i) (IS_LEFT(i) ? GET_RIGHT(GET_PARENT(i)) : GET_LEFT(GET_PARENT(i)))

typedef enum states
{
    EMPTY = 0,
    SPLIT,
    FULL,
    ALLOCATED
} states;

enum pointers
{
    MEM_START = 0x18000000,
    MEM_END = 0x1C000000,
    MAP_START = MEM_END,
    MAP_END = 0x1C100000
};

void cascade_state(char *x, size_t i, states s);
size_t round_to_power_of_two(size_t s);
void set_node_state(char *x, size_t i, states s);
states read_state(char *x, size_t i);
size_t height(size_t index);
size_t mem_size(size_t index);
size_t map_index_to_mem_index(size_t index);
size_t mem_index_to_map_index(size_t index);
/**
 * @brief dfs algorithm, searches for a node that represents the smallest unit of memory that can fit the request
 */
size_t find_buddy(size_t size, size_t index, size_t current_size);

#else

// 256 MB
#define MAP_START 0x10000000
#define MAP_SIZE HEAP_SIZE

/**
 * @brief Memory map:
 *
 * |  ...  |
 *  -------
 * |  map  |    // The map and the heap are equal sized
 *  -------
 * | heapp |    // The heap is the rest of the memory
 *  -------
 * |  ...  |
 */

/**
 * @brief The size of the heap.
 */
static const uint64_t heap_size = HEAP_SIZE;
/**
 * @brief The size of the heap's map. 1:1 with the heap size.
 */
static const uint64_t map_size = MAP_SIZE;
/**
 * @brief Start of the heap's map memory.
 */
static map_entry *const map_start = (map_entry *)MAP_START;
/**
 * @brief End of the heap's map memory (excluded).
 */
static map_entry *const map_end = (map_entry *)(MAP_START + MAP_SIZE);

#endif
#endif