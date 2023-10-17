#ifndef NSTDLIB_H
#define NSTDLIB_H

#include <stdint.h>
// syscalls
extern int print_sys(char *, int);
extern unsigned long long get_unix_time(void);

// library
char *utoa(unsigned int n, char *buffer, int radix);
char *itoa(int n, char *buffer, int radix);
char *ulltoa(uint64_t ll, char *buffer, int radix);

#endif
