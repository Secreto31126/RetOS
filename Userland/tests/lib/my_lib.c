#include <string.h>
#include "my_lib.h"
#include <sys.h>

char *strandnum(const char *str, int num)
{
    char *buffer = malloc(strlen(str) + 3);
    strcpy(buffer, str);
    itoa(num, buffer + strlen(str), 10);
    return buffer;
}