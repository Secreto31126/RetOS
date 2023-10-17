#include <stdint.h>
#include <console.h>

static uint64_t exceptions = 0;

static void zero_division();
static void invalid_opcode();

void exception_manager(int exception)
{
    exceptions++;

    switch (exception)
    {
    case 0:
        return zero_division();

    case 1:
        return invalid_opcode();

    default:
        break;
    }
}

static void zero_division()
{
    ncPrint("Zero division exception\n");
    return;
}

static void invalid_opcode()
{
    ncPrint("I like your funny words, magic man\n");
    return;
}

uint64_t get_exceptions_count()
{
    return exceptions;
}
