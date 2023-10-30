#include "interruptions.h"

static uint64_t exceptions = 0;

static void zero_division();
static void invalid_opcode();

static void (*exception_handlers[20])(void) = {
    zero_division,
    noop,
    noop,
    noop,
    noop,
    noop,
    invalid_opcode,
    noop,
    noop,
    noop,
    noop,
    noop,
    noop,
    noop,
    noop,
    noop,
    noop,
    noop,
};

void exception_manager(uint8_t exception)
{
    exceptions++;
    if (exception < 20)
    {
        exception_handlers[exception]();
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
