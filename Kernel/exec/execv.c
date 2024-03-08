#include "exec.h"

static Stack current_stack = NULL;
static RSP current_rsp = NULL;
static int current_argc = 0;

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

            pid++;

            RSP rsp = NULL;
            Stack stack_memory = getStackTopAndBase(&rsp);

            if (!stack_memory)
            {
                // ENOMEM
                return 12;
            }

            char *empty_argv[1] = {NULL};
            int argc = setStackArgs(&rsp, argv ? argv : empty_argv, executables[i]);

            if (argc < 0)
            {
                freeStack(stack_memory, rsp, 0);
                return -argc;
            }

            if (current_stack)
            {
                freeStack(current_stack, current_rsp, current_argc);
            }

            current_stack = stack_memory;
            current_rsp = rsp;
            current_argc = argc;

            portal_to_userland(executables[i].main, rsp);
        }
    }

    // ENOENT
    return 2;
}
