#ifndef NSTDLIB_H
#define NSTDLIB_H
#define MAX_DOUBLE_LENGTH 100
#define DOUBLE_RESOLUTION 3

#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#define EOF -1

// syscalls
extern int print_sys(unsigned int fd, char *str, unsigned long long len);
extern int read_sys(unsigned int fd, char *str, unsigned long long len);
extern uint64_t get_tick();
extern unsigned long long get_unix_time(void);
extern void *malloc(uint64_t size);
extern void free(void *ptr);
extern void beep(uint32_t freq);
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

// library
void *realloc(void *ptr, uint64_t oldSize, uint64_t newSize);
uint64_t atoiHex(char *s);
char putChar(char c);
char readChar();
char getChar();
int read(char *buffer, int count);
uint64_t scanf(char *format, ...);
uint64_t puts(char *string);
uint64_t printf(char *format, ...);
char *sPrintf(char *format, ...);
uint64_t sPuts(char *receiver, const char *source);
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
uint64_t concatFrom(char *sEnd, char *sAdd);
// tracks allocated strings. All tracked strings freed upon call to freePrints.
void addToAllocated(char *address);
char strcmpHandleWhitespace(char *s1, char *s2);
char *shiftToWord(char *s);
char *getDumpString();
char timeHasPassed(uint64_t start, uint64_t unit);

#endif
