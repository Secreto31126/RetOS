#ifndef SYS_H
#define SYS_H

#include <sched.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/pselect.h>
#include <sys/resource.h>

// uint64_t draw(HexColor *figure, uint64_t dimensions, uint64_t position);
extern void *pshm(uint32_t size);
extern void free_shm(void *ptr);
extern size_t get_unix_time(void);
extern size_t get_screen_size();
extern void beep(uint32_t freq);
extern size_t get_tick();
extern int fork();
extern int flush(int fd);
extern int memory_state(char *output, size_t length);

#endif
