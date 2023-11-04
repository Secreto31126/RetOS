#include "interruptions.h"

/**
 * @brief Exception counter
 */
static uint64_t exceptions = 0;
/**
 * @brief The last error code
 */
static uint64_t error_code = 0;

/**
 * @brief Handles exceptions
 *
 * @param exception The exception number
 */
void exception_manager(uint8_t exception)
{
    if (exception == 0 || exception == 6)
        exceptions++;
    error_code = exception;

    beep(440);
    BSOD(error_code);
    BSOD(error_code);
    BSOD(error_code);
    clear_screen();
}

uint64_t get_exceptions_count()
{
    return exceptions;
}

uint64_t latest_error_code()
{
    return error_code;
}
