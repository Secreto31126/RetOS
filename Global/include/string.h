#ifndef GSTR_H
#define GSTR_H

#include <stddef.h>

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

/**
 * @brief Concatenate two strings
 *
 * @param dest The destination buffer to concatenate to
 * @param src The source buffer to concatenate from
 * @return char* The destination buffer
 */
char *strcat(char *dest, const char *src);

#endif
