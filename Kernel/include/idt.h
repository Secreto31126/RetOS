#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include <lib.h>

typedef void (*InterruptionHandler)();

void clearIDT();
void *idtEntry(uint8_t interrupt_number, InterruptionHandler handler);

#endif
