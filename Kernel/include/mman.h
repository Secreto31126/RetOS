#ifndef MMN_H
#define MMN_H

#include <stddef.h>
#include <stdint.h>

#define NULL ((void *)0)

/**
 * @brief Initializes the memory manager.
 */
void init_memory_manager();
/**
 * @brief Get a stringification of the memory state
 *
 * @param output The output buffer
 * @param length The length of the output buffer
 * @return int The length of the output
 */
int memory_state(char *output, size_t length);

/**
 * @brief Reserves a block of memory of the given size.
 *
 * @param size The size of the block to reserve. Returns NULL if size is 0 or
 * over 0x8000.
 * @return void* A pointer to the reserved block of memory. Returns NULL if
 * there is not enough memory.
 */
void *malloc(uint32_t size);
/**
 * @brief Free a block of memory.
 *
 * @param ptr A pointer to the block of memory to free.
 * Freeing not from the start of the block will cause undefined behavior.
 */
void free(void *ptr);

#endif
