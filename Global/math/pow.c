#include <math.h>

unsigned long long pow(double x, int y)
{
    unsigned long long ans = 1.0;
    for (unsigned int i = 0; i < y; i++)
        ans *= x;
    return ans;
}
