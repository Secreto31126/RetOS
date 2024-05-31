#ifndef SYS_H
#define SYS_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Read from a file descriptor
 *
 * @param fd 0 for stdin, 2 for stderr, 3 for stdkey
 * @param buf The buffer to write to
 * @param count The number of bytes to read
 * @return size_t The number of bytes read
 */
extern size_t read(int fd, void *buf, size_t count);
extern size_t write(int fd, const void *str, size_t len);
// uint64_t draw(HexColor *figure, uint64_t dimensions, uint64_t position);
extern void *malloc(uint32_t size);
extern void free(void *ptr);
extern size_t get_unix_time(void);
extern size_t get_screen_size();
extern void beep(uint32_t freq);
extern size_t get_tick();
extern void halt_user();
extern void exit(int status);
extern int get_pid();
extern int execv(char *pathname, char *argv[]);
extern int fork();
extern void yield();
extern int waitpid(int pid, int *wstatus, int options);
extern void sleep(unsigned int seconds);
extern int pipe(int pipefd[2]);
extern int close(int fd);
extern int dup2(int oldfd, int newfd);
extern void usleep(unsigned int usec);

#endif
