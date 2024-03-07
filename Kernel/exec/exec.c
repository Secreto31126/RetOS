#include "exec.h"
#include <console.h>

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
    Stack stack = (Stack)(*rsp);

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

    *stack = NULL;

    // ncNewline();
    // ncPrintHex((uint64_t)stack);
    // ncPrint(": ");
    // ncPrint("NULL");
    // ncNewline();

    stack--;

    for (int i = argc - 1; i >= 0; i--)
    {
        char *args = argv[i];
        *stack = strcpy((char *)malloc(strlen(args) + 1), args);

        // ncNewline();
        // ncPrintHex(i);
        // ncPrint(" arg: ");
        // ncPrintHex((uint64_t)stack);
        // ncPrint(": ");
        // ncPrint(args);

        stack--;
    }

    // Executable name
    *stack = strcpy((char *)malloc(strlen(executable.filename) + 1), executable.filename);
    argc++;

    // ncNewline();
    // ncPrintHex((uint64_t)stack);
    // ncPrint(": ");
    // ncPrint(stack[0]);
    // ncNewline();

    // ncNewline();
    // ncPrint("rsp: ");
    // ncPrintHex(*rsp);
    // ncPrint("->");

    *rsp = (RSP)(stack);

    // ncPrintHex(*rsp);
    // ncNewline();

    return argc;
}

void freeStack(Stack stack, int argc)
{
    for (int i = 0; i < argc; i++)
        free(stack[i]);
    free(stack);
}
