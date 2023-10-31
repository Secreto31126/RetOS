#ifndef INT_H
#define INT_H

#include <stdint.h>

typedef void (*InterruptionHandler)();
void initialize_idt();

uint64_t get_exceptions_count();
/**
 * @brief Get the latest error code
 *
 * @return uint64_t The error code, trash if dump_reg_string is null
 */
uint64_t latest_error_code();

#endif
