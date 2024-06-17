#include <memory.h>

#include <stdint.h>

void *memcpy(void *dest, const void *src, size_t count) {
  /*
   * memcpy does not support overlapping buffers, so always do it
   * forwards. (Don't change this without adjusting memmove.)
   *
   * For speedy copying, optimize the common case where both pointers
   * and the length are word-aligned, and copy word-at-a-time instead
   * of byte-at-a-time. Otherwise, copy by bytes.
   *
   * The alignment logic below should be portable. We rely on
   * the compiler to be reasonably intelligent about optimizing
   * the divides and modulos out. Fortunately, it is.
   */
  uint64_t i;

  if ((uint64_t)dest % sizeof(uint32_t) == 0 &&
      (uint64_t)src % sizeof(uint32_t) == 0 && count % sizeof(uint32_t) == 0) {
    uint32_t *d = (uint32_t *)dest;
    const uint32_t *s = (const uint32_t *)src;

    for (i = 0; i < count / sizeof(uint32_t); i++)
      d[i] = s[i];
  } else {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    for (i = 0; i < count; i++)
      d[i] = s[i];
  }

  return dest;
}
