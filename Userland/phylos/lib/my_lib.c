#include "my_lib.h"
#include <string.h>
#include <stdlib.h>
#include <sys.h>

char *strandnum(const char *str, unsigned int num)
{
    char *buffer = malloc(strlen(str) + 3);
    strcpy(buffer, str);
    utoa(num, buffer + strlen(str), 10);
    return buffer;
}