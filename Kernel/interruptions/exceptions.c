#include "interruptions.h"

// /**
//  * @brief The function to call when an exception is thrown
//  */
// extern Catch catcher;
// /**
//  * @brief The stack restoration pointer
//  */
// extern uint64_t rbp_restore;
/**
 * @brief Exception counter
 */
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
}

uint64_t get_exceptions_count()
{
    return exceptions;
}
