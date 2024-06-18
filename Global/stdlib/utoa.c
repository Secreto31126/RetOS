#include <stdlib.h>

char *utoa(unsigned int n, char *buffer, int radix) {
  return ultoa((unsigned long)n, buffer, radix);
}
