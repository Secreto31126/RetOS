#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stddef.h>

void *memset(void *destination, int character, size_t length);
void *memcpy(void *destination, const void *source, size_t length);

/**
 * @brief Compare strings
 * @see https://codebrowser.dev/glibc/glibc/string/strcmp.c.html
 *
 * @param p1 First string
 * @param p2 Second string
 * @return int The distance between both strings
 */
int strcmp(const char *p1, const char *p2);
/**
 * @brief Get the length of a string
 *
 * @param str The string
 * @return size_t The length of the string
 */
size_t strlen(const char *str);
/**
 * @brief Copy src to dst, truncating or null-padding to always copy n bytes.
 * @see https://android.googlesource.com/platform/bionic/+/a27d2baa/libc/string/strncpy.c
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param n The number of bytes to copy
 * @return char* The destination buffer
 */
char *strncpy(char *dest, const char *src, size_t n);
/**
 * @brief Copy src to dest, including the terminating '\0' character.
 * @see https://android.googlesource.com/platform/bionic/+/a27d2baa/libc/string/strcpy.c
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @return char* The destination buffer
 */
char *strcpy(char *dest, const char *src);

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
