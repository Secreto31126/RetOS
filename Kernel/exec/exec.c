#include "exec.h"

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
};

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

#define PUSH(rsp, val) (*((char **)(--(rsp))) = (val))

void set_stack_args(int argc, char *const argv[], Stack heap, RSP *rsp)
{
    PUSH(*rsp, NULL);

    for (int i = argc - 1; i >= 0; i--)
    {
        char *args = argv[i];

        char *copy = heap;
        heap += strlen(args) + 1;

        strcpy(copy, args);
        PUSH(*rsp, copy);
    }
}

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

    while (argv[argc] && ++argc < MAX_ARGS)
        ;

    if (argc >= MAX_ARGS)
    {
        // E2BIG
        return -7;
    }

    return argc;
}
