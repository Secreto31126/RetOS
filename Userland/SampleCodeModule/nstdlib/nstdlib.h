#ifndef NSTDLIB_H
#define NSTDLIB_H
#define MAX_DOUBLE_LENGTH 100
#define DOUBLE_RESOLUTION 3

#include <stdint.h>
#include <stdarg.h>
#define EOF -1

// syscalls
extern int read_sys(unsigned int fd, char *str, unsigned long long len);
extern int print_sys(unsigned int fd, char *str, unsigned long long len);
extern void *malloc(uint64_t size);
extern void free(void *ptr);
extern unsigned long long get_unix_time(void);
/**
 * @brief Plays at the given frequency indefinitely. Async.
 *
 * @param frequency Frequency in Hz, 0 to stop.
 */
extern void beep(uint64_t frequency);
/**
 * @brief Get the current tick
 *
 * @return uint64_t The current tick
 */
extern uint64_t get_tick();

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
uint64_t sPrintf(char *format, ...);
uint64_t sPuts(char *receiver, char *source);
char sPutChar(char *receiver, char c);
uint64_t strlen(char *s);
static const double pi = 3.14159265359;
uint64_t pow(double base, uint64_t exponent); // can't return doubles in userland because SSE registers aren't usable
char strcmp(char *s1, char *s2);
char isPrefix(char *prefix, char *word);
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
/**
 * @brief Play an array of frequencies, one per tick. Sync
 *
 * @param song The array of frequencies
 * @param length The length of the array
 */
void play_song(uint32_t *song, uint64_t length);

#endif
