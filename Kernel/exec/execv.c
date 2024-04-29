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
                return 1;
            }

            int argc = check_args(argv);

            if (argc < 0)
            {
                return -argc;
            }

            Process *process = get_current_process();
            RSP rsp = process->stack + process->stack_size;

            set_stack_args(argc, argv, (Stack)process->stack, &rsp);

            process->rsp = rsp;

            portal_to_userland(executables[i].main, rsp);
        }
    }

    // ENOENT
    return 2;
}
