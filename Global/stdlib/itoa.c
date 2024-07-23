#include <stdlib.h>

char *itoa(int n, char *string, int radix) {
  int un = n;
  if (n < 0) {
    *string = '-';
    string++;
    un = -n;
  }

  return utoa((unsigned int)un, string, radix) - (n < 0);
}
