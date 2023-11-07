#include "exec.h"

int execv(const char *pathname, char *const argv[])
{
    for (uint64_t i = 0; i < EXECUTABLES; i++)
    {
        if (!strcmp(pathname, executables[i].filename))
        {
            if (!(executables[i].mod & 1))
            {
                // EPERM
                return -1;
            }

            // HACK: If userland returns negative, ups
            // Solution: ERRNO static variable to differentiate
            return portal_to_userland(0, argv, executables[i].main, getStackBase());
        }
    }

    // ENOENT
    return -2;
}
