#include <stdlib.h>

void exit(int status)
{
    // TODO: call at_exit functions
    // NOTE: Won't happen ¯\_(ツ)_/¯
    _exit(status);
}
