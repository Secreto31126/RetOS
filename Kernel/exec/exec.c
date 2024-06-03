#include "exec.h"

#define PUSH(rsp, val) (*((void **)(--(rsp))) = (val))

Executable executables[EXECUTABLES] = {
    {
        .mod = 7,
        .filename = "module",
        .main = (EntryPoint)0x400000,
    },
    {
        .mod = 6,
        .filename = "data",
        .main = (EntryPoint)0x500000,
    },
    {
        .mod = 7,
        .filename = "tomyland",
        .main = (EntryPoint)0x600000,
    },
    {
        .mod = 7,
        .filename = "cat",
        .main = (EntryPoint)0x605000,
    },
    {
        .mod = 7,
        .filename = "wc",
        .main = (EntryPoint)0x610000,
    },
    {
        .mod = 7,
        .filename = "filter",
        .main = (EntryPoint)0x615000,
    },
    {
        .mod = 7,
        .filename = "loop",
        .main = (EntryPoint)0x620000,
    },
    {
        .mod = 7,
        .filename = "grep",
        .main = (EntryPoint)0x625000,
    },
    {
        .mod = 7,
        .filename = "sing",
        .main = (EntryPoint)0x630000,
    },
    {
        .mod = 7,
        .filename = "less",
        .main = (EntryPoint)0x635000,
    },
};

int check_args(char *const argv[])
{
    /**
     * @brief The number of arguments, does not include the NULL terminator
     */
    int argc = 0;

    if (!argv)
    {
        return argc;
    }

    while (argv[argc] && argc + 1 < MAX_ARGS)
    {
        if (strlen(argv[argc++]) >= MAX_ARG_LEN - 1)
        {
            // E2BIG
            return -7;
        }
    }

    if (argc >= MAX_ARGS)
    {
        // E2BIG
        return -7;
    }

    return argc;
}

RSP set_process_args(int argc, char const argv[MAX_ARGS][MAX_ARG_LEN], Process *process)
{
    RSP *rsp = (RSP *)&process->rsp;
    Stack heap = process->stack;

    *rsp = process->stack + process->stack_size;

    PUSH(*rsp, NULL);

    for (int i = argc - 1; i >= 0; i--)
    {
        char const *args = argv[i];

        char *copy = heap;
        heap += strlen(args) + 1;

        strcpy(copy, args);
        PUSH(*rsp, copy);
    }

    // "initialization of 'void *' from 'int' makes pointer from integer without a cast"
    // Take that, GCC. Hope you are happy with this monstrosity.
    PUSH(*rsp, NULL + argc);

    return *rsp;
}

char argv_backup[MAX_ARGS][MAX_ARG_LEN];

char *const *const backup_argv_somewhere(int argc, char *const argv[])
{
    for (int i = 0; i < argc; i++)
    {
        strcpy(argv_backup[i], argv[i]);
    }

    return (char *const *const)argv_backup;
}
