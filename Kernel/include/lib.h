#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);
/**
 * @brief Compare strings
 * @see https://codebrowser.dev/glibc/glibc/string/strcmp.c.html
 *
 * @param p1 First string
 * @param p2 Second string
 * @return int The distance between both strings
 */
int strcmp(const char *p1, const char *p2);

extern char *cpuVendor(char *result);

extern uint32_t input_dword(uint16_t port);
extern void output_dword(uint16_t port, uint32_t value);
extern uint8_t input_byte(uint16_t port);
extern void output_byte(uint16_t port, uint8_t value);

extern void set_interrupt_flag(void);
extern void unset_interrupt_flag(void);
extern void halt_once(void);

extern char dump_reg_string[360];
/**
 * @brief Dump registers value into dump_reg_string
 *
 * @return uint64_t The string length
 */
extern uint64_t dump_regs(void);

#endif
