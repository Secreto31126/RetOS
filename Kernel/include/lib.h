#ifndef LIB_H
#define LIB_H

#include <memory.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Send QEMU a shutdown signal, or halt the CPU if failed
 */
void power_off(void);
/**
 * @brief Change the endianess of a 16 bit value
 * 
 * @param val The value to change
 * @return uint16_t The changed value
 */
uint16_t endian_word(uint16_t val);
/**
 * @brief Change the endianess of a 32 bit value
 * 
 * @param val The value to change
 * @return uint32_t The changed value
 */
uint32_t endian_dword(uint32_t val);

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
