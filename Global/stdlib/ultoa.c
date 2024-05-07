#include <stdlib.h>

char *ultoa(unsigned long l, char *buffer, int radix)
{
    char *toRet = buffer;
    while (l >= radix)
    {
        *buffer = (l % radix) + '0';
        if (*buffer > '9')
            *buffer += 'A' - '9' - 1;
        buffer++;
        l /= radix;
    }

    *buffer = l + '0';
    if (*buffer > '9')
        *buffer += 'A' - '9' - 1;
    *(buffer + 1) = 0;
    int halfLen = (buffer - toRet) / 2;
    for (int i = 0; i <= halfLen; i++)
    {
        char aux = *(toRet + i);
        *(toRet + i) = *(buffer - i);
        *(buffer - i) = aux;
    }

    return toRet;
}
