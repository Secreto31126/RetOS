#ifndef GLIB_H
#define GLIB_H

#include <unistd.h>

#define ISSPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\v' || (c) == '\f' || (c) == '\r')
#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')
#define ISALPHA(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define ISUPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define ISLOWER(c) ((c) >= 'a' && (c) <= 'z')

#define NULL ((void *)0)
#define READ_END 0
#define WRITE_END 1
#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2
#define STD_KEYS 3
#define STD_TERM 4

char *itoa(int n, char *buffer, int radix);
char *utoa(unsigned int n, char *buffer, int radix);
char *ultoa(unsigned long l, char *buffer, int radix);
/**
 * @brief Convert a string to an integer
 */
int atoi(const char *string);
/**
 * @brief Convert a string to a long integer.
 *
 * @note Ignores 'locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 *
 * @see https://github.com/gcc-mirror/gcc/blob/master/libiberty/strtol.c
 */
long strtol(const char *nptr, char **endptr, register int base);
/**
 * @brief Convert a string to an unsigned long long integer.
 *
 * @note Ignores 'locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 *
 * @see https://github.com/gcc-mirror/gcc/blob/master/libiberty/strtoull.c
 */
unsigned long long strtoull(const char *nptr, char **endptr, register int base);
/**
 * @brief Kill the current process, calling the exit handlers.
 * @note There's no way to assign a handler to be called at exit :]
 */
void exit(int status);

#endif
