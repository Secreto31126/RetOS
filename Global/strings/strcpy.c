#include <strings.h>

char *strcpy(char *dest, const char *src)
{
    char *save = dest;
    for (; (*dest = *src); ++src, ++dest)
        ;
    return save;
}
