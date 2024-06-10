#include <memory.h>

#include <stdint.h>

void *memset(void *dest, int c, size_t count)
{
    uint8_t chr = (uint8_t)c;
    char *dst = (char *)dest;

    while (count--)
        dst[count] = chr;

    return dest;
}
