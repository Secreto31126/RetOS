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

    BSOD(exception);
    set_interrupt_flag();
    sleep_ticks(18 * 3);
    unset_interrupt_flag();

    // If weird exception, suicide
    if (exception != 0 && exception != 6)
    {
        while (1)
        {
            unset_interrupt_flag();
            halt_once();
        }
    }

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
