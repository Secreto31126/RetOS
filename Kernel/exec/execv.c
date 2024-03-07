#include "exec.h"
#include <console.h>

static Stack current_stack = NULL;
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

            RSP rsp;
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
                freeStack(stack_memory, 0);
                return -argc;
            }

            // ncPrint("Hi :]\n");
            if (current_stack)
            {
                // ncPrint("Uh :[\n");
                freeStack(current_stack, current_argc);
            }

            // ncPrint("Hehe :]\n");
            current_argc = argc;
            current_stack = stack_memory;

            // ncPrint("Byeeee :]\n");
            portal_to_userland(executables[i].main, rsp);
        }
    }

    // ENOENT
    return 2;
}
