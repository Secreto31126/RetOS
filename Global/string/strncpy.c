#include <string.h>

char *strncpy(char *dest, const char *src, size_t n) {
  if (n) {
    char *d = dest;
    const char *s = src;

    do {
      if (!(*d++ = *s++)) {
        /* NUL pad the remaining n-1 bytes */
        while (--n)
          *d++ = 0;
        break;
      }
    } while (--n);
  }

  return dest;
}
