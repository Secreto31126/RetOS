#ifndef STDIN_H
#define STDIN_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Read from stdin
 *
 * @param buf The direction to store the data
 * @param len The number of bytes to read
 * @return uint16_t The number of bytes read
 */
uint16_t read_stdin(uint8_t *buf, uint16_t len);
/**
 * @brief Write to stdin
 *
 * @param buf The data to write
 * @param len The number of bytes to write
 * @return uint16_t The number of bytes written
 */
uint16_t write_stdin(uint8_t *buf, uint16_t len);
/**
 * @brief Wether or not stdin is empty
 *
 * @return true stdin is empty
 * @return false stdin is not empty
 */
bool stdin_empty();

#endif
