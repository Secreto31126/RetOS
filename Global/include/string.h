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
 * @brief Compare two strings up to n characters
 * @see https://github.com/gcc-mirror/gcc/blob/master/libiberty/strncmp.c
 *
 * @param s1 The first string
 * @param s2 The second string
 * @param n The number of characters to compare
 * @return int The distance between both strings
 */
int strncmp(const char *s1, const char *s2, register size_t n);

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

/**
 * @brief Find the index of a substring in a string
 * @see https://android.googlesource.com/platform/bionic/+/ics-mr0/libc/string/strstr.c
 *
 * @param s The string to search in
 * @param find The substring to search for
 * @return char* The index of the substring in the string
 */
char *strstr(const char *s, const char *find);

#endif
