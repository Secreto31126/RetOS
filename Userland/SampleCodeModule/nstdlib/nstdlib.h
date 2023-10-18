#ifndef NSTDLIB_H
#define NSTDLIB_H
#define MAX_DOUBLE_LENGTH 100
#define DOUBLE_RESOLUTION 3

#include <stdint.h>
#include <stdarg.h>
#define EOF -1

// syscalls
extern int print_sys(char *, int);
extern int read_sys(char *, int);
extern unsigned long long get_unix_time(void);

// library
char *utoa(unsigned int n, char *buffer, int radix);
char *itoa(int n, char *buffer, int radix);
char *ultoa(uint64_t ll, char *buffer, int radix);
char putChar(char c);
uint64_t puts(char *string);
uint64_t printf(char *format, ...);
uint64_t strlen(char *s);

#endif
