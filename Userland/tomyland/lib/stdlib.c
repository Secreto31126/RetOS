#include <lib.h>

void *memset(void *destiation, int c, size_t length)
{
    char chr = (char)c;
    char *dst = (char *)destiation;

    while (length--)
        dst[length] = chr;

    return destiation;
}
