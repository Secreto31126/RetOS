#ifndef PMEM_H
#define PMEM_H

#include <memory.h>

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
