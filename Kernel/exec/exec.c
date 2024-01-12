#include "exec.h"

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

static const uint64_t PageSize = 0x1000;

void *getStackBase()
{
    // The size of the stack itself, 32KiB, begin at the top of the stack
    return (void *)((uint64_t)&endOfKernel + PageSize * 8 - sizeof(uint64_t));
}
