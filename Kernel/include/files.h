#ifndef FLE_H
#define FLE_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Read from a file descriptor
 *
 * @param fd The file descriptor to read from
 * @param buffer The buffer to write to
 * @param count The number of bytes to read
 * @return size_t The number of bytes read
 */
size_t read(int fd, void *buffer, size_t count);
/**
 * @brief Write to a file descriptor
 *
 * @param fd The file descriptor to write to
 * @param buffer The buffer to write from
 * @param count The number of bytes to write
 * @return size_t The number of bytes written
 */
size_t write(int fd, const void *buffer, size_t count);
/**
 * @brief Check if a file would block at reading
 *
 * @param file The file to check
 * @return true it blocks
 * @return false it doesn't
 */
bool would_block(int file);

#endif
