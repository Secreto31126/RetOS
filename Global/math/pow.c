#include <math.h>

unsigned long long pow(double x, int y) {
  double ans = 1.0;
  while (y--)
    ans *= x;
  return (unsigned long long)ans;
}
