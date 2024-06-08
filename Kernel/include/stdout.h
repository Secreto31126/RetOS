#ifndef STDOUT_H
#define STDOUT_H

#include <stdint.h>
#include <stdbool.h>
#include <semaphores.h>

/**
 * @brief Set up stdout
 *
 * @return int 0 on success, -1 on failure
 */
int init_stdout();
/**
 * @brief Read from stdout
 *
 * @param buf The direction to store the data
 * @param len The number of bytes to read
 * @return uint16_t The number of bytes read
 */
uint16_t read_stdout(uint8_t *buf, uint16_t len);
/**
 * @brief Write to stdout
 *
 * @param buf The data to write
 * @param len The number of bytes to write
 * @return uint16_t The number of bytes written
 */
uint16_t write_stdout(uint8_t *buf, uint16_t len);
/**
 * @brief Wether or not stdout is empty
 *
 * @return true stdout is empty
 * @return false stdout is not empty
 */
bool stdout_empty();
/**
 * @brief Flush stdout
 */
void flush_stdout();

#endif
