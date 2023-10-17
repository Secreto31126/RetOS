#ifndef IDT_H
#define IDT_H

#include <lib.h>
#include <stdint.h>

typedef void (*InterruptionHandler)();
void initialize_idt();

uint64_t get_exceptions_count();

#endif
