#ifndef NSTDLIB_H
#define NSTDLIB_H
#define MAX_DOUBLE_LENGTH 100
#define DOUBLE_RESOLUTION 3

#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/pselect.h>
#define EOF ((char)-1)
#define LCTRL ((char)0x1D)

// syscalls
#define read_sys read
#define print_sys write
extern uint64_t get_tick();
extern unsigned long long get_unix_time(void);
extern void *malloc(uint64_t size);
extern void free(void *ptr);
extern void beep(uint32_t freq);
extern int fork();
extern int flush(int fd);
extern int memory_state(char *output, size_t length);

// library
void *realloc(void *ptr, uint64_t oldSize, uint64_t newSize);
uint64_t atoiHex(char *s);
char putChar(char c);
char readChar();
char getChar();
uint64_t scanf(char *format, ...);
uint64_t puts(char *string);
uint64_t printf(char *format, ...);
char *sPrintf(char *format, ...);
uint64_t sPuts(char *receiver, const char *source);
char sPutChar(char *receiver, char c);
static const double pi = 3.14159265359;
uint64_t pow(double base, uint64_t exponent); // can't return doubles in userland because SSE registers aren't usable
char isPrefix(char *prefix, char *word);
char isFirstWord(char *s1, char *firstWord);
uint64_t getHours();
uint64_t getMinutes();
char *getTimeString(char *buffer);
void freePrints();
char *concatUnlimited(char *s1, char *s2);
uint64_t concatFrom(char *sEnd, char *sAdd);
// tracks allocated strings. All tracked strings freed upon call to freePrints.
void addToAllocated(char *address);
char strcmpHandleWhitespace(char *s1, char *s2);
char *shiftToWord(char *s);
char *shiftToNextWord(char *s);
int readNFromFd(int fd, char *buffer, int n);
char *getDumpString();
char timeHasPassed(uint64_t start, uint64_t unit);
void separateString(char *s, char **buffer, int bufferSize);
int isLastAlpha(const char *s, char alpha);

#endif
