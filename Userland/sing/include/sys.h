#ifndef SYS_H
#define SYS_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/pselect.h>
#include <sys/wait.h>
#include <unistd.h>

// uint64_t draw(HexColor *figure, uint64_t dimensions, uint64_t position);
extern void *pshm(uint32_t size);
extern void free(void *ptr);
extern size_t get_unix_time(void);
extern size_t get_screen_size();
extern void beep(uint32_t freq);
extern size_t get_tick();
extern int fork();
extern int flush(int fd);

#endif
