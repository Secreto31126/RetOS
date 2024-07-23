#ifndef LIB_H
#define LIB_H

#include <memory.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Send QEMU a shutdown signal, or halt the CPU if failed
 */
void power_off(void);

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
extern unsigned int exchange(unsigned int *src, unsigned int value);

extern unsigned char dump_reg_string[360];
/**
 * @brief Dump registers value into dump_reg_string
 */
extern void dump_regs(void);

#endif
