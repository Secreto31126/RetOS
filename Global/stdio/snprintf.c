#include <stdio.h>

int snprintf(char *str, size_t size, const char *format, ...)
{
    int r;
    va_list args;
    va_start(args, format);
    r = vsnprintf(str, size, format, args);
    va_end(args);
    return r;
}
