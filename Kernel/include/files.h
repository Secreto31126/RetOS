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
 * @brief Flush a file descriptor
 * @note Fails if the file is a pipe
 *
 * @param fd The file descriptor to check
 * @return 0 on success, -1 on failure
 */
int flush(int fd);
/**
 * @brief Check if a file is empty
 *
 * @param file The file to check
 * @return true it is
 * @return false it isn't
 */
bool file_empty(int file);
/**
 * @brief Check if a file is full
 *
 * @param file The file to check
 * @return true it is
 * @return false it isn't
 */
bool file_full(int file);

#endif
