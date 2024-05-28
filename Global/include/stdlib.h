#ifndef GLIB_H
#define GLIB_H

#ifndef ULLONG_MAX
#define ULLONG_MAX (~(unsigned long long)0) /* 0xFFFFFFFFFFFFFFFF */
#endif

#ifndef ULONG_MAX
#define ULONG_MAX ((unsigned long)(~0L)) /* 0xFFFFFFFF */
#endif

#ifndef LONG_MAX
#define LONG_MAX ((long)(ULONG_MAX >> 1)) /* 0x7FFFFFFF */
#endif

#ifndef LONG_MIN
#define LONG_MIN ((long)(~LONG_MAX)) /* 0x80000000 */
#endif

#define ISSPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\v' || (c) == '\f' || (c) == '\r')
#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')
#define ISALPHA(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define ISUPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define ISLOWER(c) ((c) >= 'a' && (c) <= 'z')

#define NULL ((void *)0)

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

#endif
