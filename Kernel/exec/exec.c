#include "exec.h"
// #include <console.h>

int pid = 0;

Executable executables[EXECUTABLES] = {
    {
        .mod = 7,
        .filename = "module",
        .main = (EntryPoint)0x400000,
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

Stack getStackTopAndBase(RSP *rsp)
{
    Stack stackTop = malloc(STACK_SIZE);
    if (!stackTop)
    {
        return NULL;
    }

    *rsp = (RSP)(((uint8_t *)stackTop) + STACK_SIZE - sizeof(uint64_t));
    return stackTop;

    // return (void *)((uint64_t)&endOfKernel + PageSize * 8 - sizeof(uint64_t));
}

int setStackArgs(RSP *rsp, char *const argv[], Executable executable)
{
    *(Stack)(*rsp) = NULL;

    /**
     * @brief The number of arguments, does not include the NULL terminator
     */
    int argc = 0;
    while (argv[argc] && ++argc < MAX_ARGS)
        ;

    if (argc >= MAX_ARGS)
    {
        // E2BIG
        return -7;
    }

    // ncNewline();
    // ncPrint("argc: ");
    // ncPrintDec(argc);
    // ncNewline();
    // ncPrintHex((uint64_t)*rsp);
    // ncPrint(": ");
    // ncPrint("NULL");
    // ncNewline();

    (*rsp)--;

    for (int i = argc - 1; i >= 0; i--)
    {
        char *args = argv[i];

        // ncNewline();
        // ncPrintHex((uint64_t)args);
        // ncPrint(" - arg ");
        // ncPrintDec(i);
        // ncPrint(": ");

        char *copy = malloc(strlen(args) + 1);

        if (!copy)
        {
            freeStack(NULL, ++(*rsp));
            return -12;
        }

        *(Stack)(*rsp) = strcpy(copy, args);

        // ncPrintHex((uint64_t)*rsp);
        // ncPrint(": ");
        // ncPrint(args);

        (*rsp)--;
    }

    // Executable name
    char *copy = malloc(strlen(executable.filename) + 1);

    if (!copy)
    {
        freeStack(NULL, ++(*rsp));
        return -12;
    }

    *(Stack)(*rsp) = strcpy(copy, executable.filename);
    argc++;

    // ncNewline();
    // ncPrintHex((uint64_t)*rsp);
    // ncPrint(": ");
    // ncPrint(*(Stack)(*rsp));
    // ncNewline();

    return argc;
}

void freeStack(Stack stack, RSP rsp)
{
    while (rsp && *rsp)
        free(*rsp++);
    free(stack);
}
