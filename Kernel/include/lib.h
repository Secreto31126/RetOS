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
extern uint16_t input_word(uint16_t port);
extern void output_word(uint16_t port, uint16_t value);
extern uint8_t input_byte(uint16_t port);
extern void output_byte(uint16_t port, uint8_t value);

extern void set_interrupt_flag(void);
extern void unset_interrupt_flag(void);
extern void halt_once(void);

extern unsigned char dump_reg_string[360];
/**
 * @brief Dump registers value into dump_reg_string
 */
extern void dump_regs(void);

#endif
