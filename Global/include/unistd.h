#ifndef GUNI_H
#define GUNI_H

#include <sys/types.h>
#include <stdint.h>

/**
 * @brief Read from a file descriptor
 *
 * @param fd The file descriptor to read from
 * @param buffer The buffer to write to
 * @param count The number of bytes to read
 * @return size_t The number of bytes read
 */
extern ssize_t read(int fd, void *buffer, size_t count);
/**
 * @brief Write to a file descriptor
 *
 * @param fd The file descriptor to write to
 * @param buffer The buffer to write from
 * @param count The number of bytes to write
 * @return size_t The number of bytes written
 */
extern ssize_t write(int fd, const void *buffer, size_t count);
/**
 * @brief Creates a pipe
 *
 * @param pipefd Where to store the read and write ends of the pipes
 * @return int 0 on success, else -1
 */
extern int pipe(int pipefd[2]);
/**
 * @brief Close a file in the current process
 *
 * @param fd The process file descriptor
 * @return int 0 if the file was closed, -1 otherwise
 */
extern int close(int fd);
/**
 * @brief Duplicate a file descriptor
 *
 * @param oldfd The old file descriptor to duplicate
 * @param newfd The new file descriptor to use
 * @return int
 */
extern int dup2(int oldfd, int newfd);
/**
 * @brief Starts an executable file overwriting the current process
 *
 * @param pathname The executable's pathname
 * @param argv The arguments to start the executable with, it must be null terminated, it may be null
 * @return int error code, does not return if success
 */
extern int execv(const char *pathname, char *const argv[]);
/**
 * @brief Get the currently executing pid
 *
 * @return pid_t The current pid
 */
extern pid_t getpid();
/**
 * @brief sbrk() increments the program's data space by increment bytes.
 * Calling sbrk() with an increment of 0 can be used to find the current location of the program break.
 * @note In RetOS, there's no such thing as increasing the data segment.
 * The only valid value for increment is 0, which returns the top of the stack.
 *
 * @param increment The number of bytes to increment the data segment by (0 in RetOS)
 * @return void* The new top of the stack, or -1 on error
 */
extern void *sbrk(intptr_t increment);
/**
 * @brief Block the current process for a number of ticks
 *
 * @param ticks The number of ticks to sleep
 * @return unsigned int The number of ticks left to sleep (always 0 in RetOS)
 */
extern unsigned int sleep(unsigned int ticks);
/**
 * @brief Block the current process for a number of microseconds (aka ticks)
 *
 * @param usec The number of microseconds to sleep
 * @return unsigned int 0 on success, -1 on error (never happens in RetOS)
 */
extern unsigned int usleep(unsigned int usec);
/**
 * @brief Kill the current process
 * @note This function should only be called from a syscall for "atomicity"
 */
extern void _exit(int status);

#endif
