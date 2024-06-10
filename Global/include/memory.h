#ifndef GMEM_H
#define GMEM_H

#include <stddef.h>

void *memset(void *dest, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t count);

#endif
