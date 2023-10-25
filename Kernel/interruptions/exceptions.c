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

void exception_manager()
{
    exceptions++;
}

uint64_t get_exceptions_count()
{
    return exceptions;
}
