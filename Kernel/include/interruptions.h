#ifndef INT_H
#define INT_H

#include <stdint.h>

typedef void (*InterruptionHandler)();
void initialize_idt();

uint64_t get_exceptions_count();

#endif
