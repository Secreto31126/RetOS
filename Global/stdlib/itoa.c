#include <stdlib.h>

char *itoa(int n, char *string, int radix)
{
    if (n < 0)
    {
        *string = '-';
        string++;
        n = -n;
    }

    return utoa((unsigned int)n, string, radix);
}
