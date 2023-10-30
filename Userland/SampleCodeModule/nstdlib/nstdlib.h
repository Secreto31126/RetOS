#ifndef NSTDLIB_H
#define NSTDLIB_H
#define MAX_DOUBLE_LENGTH 100
#define DOUBLE_RESOLUTION 3

#include <stdint.h>
#include <stdarg.h>
#define EOF -1

// syscalls
extern int print_sys(unsigned int fd, char *str, unsigned long long len);
extern int read_sys(unsigned int fd, char *str, unsigned long long len);
extern unsigned long long get_unix_time(void);
extern void *malloc(uint64_t size);
void free(void *ptr);

// library
void *realloc(void *ptr, uint64_t oldSize, uint64_t newSize);
char *utoa(unsigned int n, char *buffer, int radix);
char *itoa(int n, char *buffer, int radix);
char *ultoa(uint64_t ll, char *buffer, int radix);
char putChar(char c);
char readChar();
char getChar();
int read(char *buffer, int count);
uint64_t scanf(char *format, ...);
uint64_t puts(char *string);
uint64_t printf(char *format, ...);
uint64_t strlen(char *s);
static const double pi = 3.14159265359;
uint64_t pow(double base, uint64_t exponent); // can't return doubles in userland because SSE registers aren't usable
char strcmp(char *s1, char *s2);
void wait();

#endif
