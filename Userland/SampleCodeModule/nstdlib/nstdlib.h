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
extern uint64_t get_tick();
extern unsigned long long get_unix_time(void);
extern void *malloc(uint64_t size);
extern void free(void *ptr);
extern void beep(uint32_t freq);

// library
void *realloc(void *ptr, uint64_t oldSize, uint64_t newSize);
char *utoa(unsigned int n, char *buffer, int radix);
char *itoa(int n, char *buffer, int radix);
char *ultoa(uint64_t ll, char *buffer, int radix);
uint64_t atoi(char *s);
uint64_t atoiHex(char *s);
char putChar(char c);
char readChar();
char getChar();
int read(char *buffer, int count);
uint64_t scanf(char *format, ...);
uint64_t puts(char *string);
uint64_t printf(char *format, ...);
char *sPrintf(char *format, ...);
uint64_t sPuts(char *receiver, char *source);
char sPutChar(char *receiver, char c);
uint64_t strlen(char *s);
static const double pi = 3.14159265359;
uint64_t pow(double base, uint64_t exponent); // can't return doubles in userland because SSE registers aren't usable
char strcmp(char *s1, char *s2);
char isPrefix(char *prefix, char *word);
char isFirstWord(char *s1, char *firstWord);
void wait();
uint64_t getHours();
uint64_t getMinutes();
char *getTimeString();
void freePrints();
char *concatUnlimited(char *s1, char *s2);
// tracks allocated strings. All tracked strings freed upon call to freePrints.
void addToAllocated(char *address);
char strcmpHandleWhitespace(char *s1, char *s2);
char *shiftToWord(char *s);
char *getDumpString();
char timeHasPassed(uint64_t start, uint64_t unit);
void sleep(uint64_t ticks);

#endif
