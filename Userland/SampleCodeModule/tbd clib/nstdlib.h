#ifndef NSTDLIB_H
#define NSTDLIB_H

#include <stdint.h>

char *utoa(unsigned int n, char *buffer, int radix);
char *itoa(int n, char *buffer, int radix);
char *ulltoa(uint64_t ll, char *buffer, int radix);

#endif
