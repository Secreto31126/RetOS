#include <nstdlib.h>

char *ulltoa(uint64_t ll, char *buffer, uint64_t radix)
{
    while (ll > radix)
    {
        *buffer = (ll % radix) + '0';
        if (*buffer > '9')
            *buffer += 'A' - '9' - 1;
        buffer++;
        ll /= radix;
    }

    *buffer = (ll % radix) + '0';
    if (*buffer > '9')
        *buffer += 'A' - '9' - 1;
    buffer++;
    ll /= radix;

    *buffer = 0;
    return buffer;
}

char *utoa(unsigned int n, char *buffer, int radix)
{
    return ulltoa(n, buffer, radix);
}

char *itoa(int n, char *buffer, int radix)
{
    if (n < 0)
    {
        *buffer = '-';
        buffer++;
        n = -n;
    }

    return utoa(n, buffer, radix);
}
