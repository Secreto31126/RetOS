#include <math.h>

unsigned long long pow(double x, int y)
{
    int ans = 1.0;
    for (int i = 0; i < y; i++)
        ans *= x;
    return ans;
}
