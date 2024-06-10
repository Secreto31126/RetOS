#include <stdlib.h>

#include <stddef.h>

int atoi(const char *nptr)
{
    return (int)strtol(nptr, (char **)NULL, 10);
}
