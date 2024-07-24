#include <memory.h>

void *memcpy(void *dest, const void *src, size_t count)
{
    return memmove(dest, src, count);
}
