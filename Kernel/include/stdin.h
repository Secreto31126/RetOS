#ifndef STDIN_H
#define STDIN_H

#include <semaphores.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Set up stdin
 *
 * @return int 0 on success, -1 on failure
 */
int init_stdin();
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
/**
 * @brief Flush stdin
 */
void flush_stdin();

#endif
