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
                return 13;
            }

            int argc = check_args(argv);

            if (argc < 0)
            {
                return -argc;
            }

            portal_to_userland(
                argc,
                backup_argv_somewhere(argc, argv),
                get_current_process(),
                executables[i].main);
        }
    }

    // ENOENT
    return 2;
}
