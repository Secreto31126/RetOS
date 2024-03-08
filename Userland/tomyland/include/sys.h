#ifndef SYS_H
#define SYS_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Read from a file descriptor
 *
 * @param fd 0 for stdin, 2 for stderr, 3 for stdkey
 * @param buffer The buffer to write to
 * @param count The number of bytes to read
 * @return size_t The number of bytes read
 */
extern size_t read(int fd, void *str, size_t len);
extern size_t write(int fd, const void *str, size_t len);
// uint64_t draw(HexColor *figure, uint64_t dimensions, uint64_t position);
extern void *malloc(uint32_t size);
extern void free(void *ptr);
extern size_t get_unix_time(void);
// uint64_t get_screen_size();
extern void beep(uint32_t freq);
extern size_t get_tick();
extern void halt_user();

#endif
