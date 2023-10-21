#ifndef PMEM_H
#define PMEM_H

#include <memory.h>

typedef uint32_t heap_entry;
typedef uint32_t map_entry;

// 256 MB
#define MAP_START 0x10000000
// 128 MB
#define HEAP_SIZE 0x8000000
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
static const uint64_t map_size = HEAP_SIZE;
/**
 * @brief Start of the heap's map.
 */
static map_entry *const map_start = (map_entry *)MAP_START;
/**
 * @brief The start of the heap.
 */
static heap_entry *const heap_start = (heap_entry *)MAP_START + MAP_SIZE;

#endif
