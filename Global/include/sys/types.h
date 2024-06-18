#ifndef GTPE_H
#define GTPE_H

#include <limits.h>
#include <stddef.h>

/**
 * @brief Used for a count of bytes or an error indication.
 *
 */
typedef long int ssize_t;
/**
 * @brief A resource identifier type
 * @note Only used for pid_t for now
 */
typedef unsigned int id_t;
/**
 * @brief An unique process identifier type
 * @note -1 indicates an invalid pid
 */
typedef int pid_t;

#endif
