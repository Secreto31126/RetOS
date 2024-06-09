#include "my_lib.h"
#include <string.h>
#include <stdlib.h>
#include <sys.h>

// This function lets you create a new (consistent) name for a sem without appending a number to the original string, which would require a buffer or memory allocation
void rot_n(char *s, unsigned int num)
{
    while (*s)
        *(s++) += num;
}