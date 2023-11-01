#ifndef STDIN_H
#define STDIN_H

#include <stdint.h>

/**
 * @brief Read from stderr
 *
 * @param buf The direction to store the data
 * @param len The number of bytes to read
 * @return uint16_t The number of bytes read
 */
uint16_t read_stderr(uint8_t *buf, uint16_t len);
/**
 * @brief Write to stderr
 *
 * @param buf The data to write
 * @param len The number of bytes to write
 * @return uint16_t The number of bytes written
 */
uint16_t write_stderr(uint8_t *buf, uint16_t len);

#endif
