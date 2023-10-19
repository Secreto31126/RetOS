#ifndef STDIN_H
#define STDIN_H

#include <stdint.h>

/**
 * @brief Read from stdin
 *
 * @param buf The direction to store the data
 * @param len The number of bytes to read
 */
void read_stdin(uint8_t *buf, uint16_t len);
/**
 * @brief Write to stdin
 *
 * @param buf The data to write
 * @param len The number of bytes to write
 */
void write_stdin(uint8_t *buf, uint16_t len);

#endif
