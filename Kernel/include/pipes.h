#ifndef PIP_H
#define PIP_H

#include <stddef.h>
#include <stdbool.h>
#include <proc.h>

#define IS_PIPE(x) ((x) & O_PIPE)

/**
 * @brief Creates a pipe
 *
 * @param pipefd Where to store the read and write ends of the pipes
 * @return int 0 on success, else -1
 */
int pipe(int pipefd[2]);
/**
 * @brief Add an end to a pipe
 *
 * @param file The pipe to add an end to
 */
void add_pipe_end(int file);
/**
 * @brief Close a pipe
 *
 * @param file The pipe to close
 * @return int 0 on success, else -1
 */
int close_pipe(int file);
/**
 * @brief Read a pipe
 *
 * @param file The pipe to read from
 * @param buf Where to store the read data
 * @param count The amount of bytes to read
 * @return int The amount of bytes read
 */
int read_pipe(int file, void *buf, size_t count);
/**
 * @brief Write to a pipe
 *
 * @param file The pipe to write to
 * @param buf The data to write
 * @param count The amount of bytes to write
 * @return int The amount of bytes written
 */
int write_pipe(int file, const void *buf, size_t count);
/**
 * @brief Wether or not a pipe has data to read
 *
 * @param file The pipe to check for
 * @return true The pipe is empty
 * @return false The pipe is not empty
 */
bool pipe_empty(int file);
/**
 * @brief Wether or not a pipe is full
 *
 * @param file The pipe to check for
 * @return true The pipe is full
 * @return false The pipe is not full
 */
bool pipe_full(int file);

#endif
