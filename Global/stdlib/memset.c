#include <stdlib.h>
#include <stdint.h>

void *memset(void *destination, int c, size_t length)
{
    uint8_t chr = (uint8_t)c;
    char *dst = (char *)destination;

    while (length--)
        dst[length] = chr;

    return destination;
}
