#ifndef GSIO_H
#define GSIO_H

#include <sys/types.h>
#include <stdarg.h>

/**
 * @brief Very portable snprintf implementation, limited in functionality,
 * esp. for %[capital] %[nonportable] %m and so on.  Reduced float functionality,
 * mostly in formatting and range (e+-16), for %f and %g.
 *
 * @note
 * %s, %d, %u, %i, %x, %c, %n and %% are fully supported.
 *   This includes width, precision, flags 0- +, and *(arg for wid,prec).
 * %f, %g, %m, %p have reduced support, support for wid,prec,flags,*, but
 *   less floating point range, no %e formatting for %g.
 *
 * @see https://opensource.apple.com/source/network_cmds/network_cmds-511/unbound/compat/snprintf.c
 *
 * @param str String buffer for result. result will be null terminated.
 * @param size Size of the buffer. null is put inside buffer.
 * @param format printf() format string.
 * @param ... arguments to print.
 * @return int number of characters. a null is printed after this.
 */
int snprintf(char *str, size_t size, const char *format, ...);

/**
 * @brief Print to string.
 *
 * @note
 * supported format specifiers:
 * 	%s, %u, %d, %x, %i, %f, %g, %c, %p, %n.
 * 	length: l, ll (for d, u, x).
 * 	precision: 6.6d (for d, u, x)
 * 		%f, %g precisions, 0.3f
 * 		%20s, '.*s'
 * 	and %%.
 *
 * @see https://opensource.apple.com/source/network_cmds/network_cmds-511/unbound/compat/snprintf.c
 * 
 * @param str String buffer for result. result will be null terminated.
 * @param size Size of the buffer. null is put inside buffer.
 * @param format printf() format string.
 * @param arg arguments to print.
 * @return number of characters. a null is printed after this.
 * @return number of bytes that would have been written
 *	   if the buffer had been large enough.
 */
int vsnprintf(char *str, size_t size, const char *format, va_list arg);

#endif
