#ifndef FLE_H
#define FLE_H

#include <stdbool.h>
#include <stddef.h>

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
