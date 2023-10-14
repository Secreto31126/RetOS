#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);

char *cpuVendor(char *result);

uint32_t input_dword(uint16_t port);
void output_dword(uint16_t port, uint32_t value);
uint8_t input_byte(uint16_t port);
void output_byte(uint16_t port, uint8_t value);

void syscall_handler(void);

#endif
