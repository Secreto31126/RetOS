#include "my_lib.h"
#include <string.h>
#include <stdlib.h>
#include <sys.h>

void rot_n(char *s, unsigned int num)
{
    while (*s)
        *(s++) += num;
}