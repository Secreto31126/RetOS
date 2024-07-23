#ifndef STDERR_H
#define STDERR_H

#include <semaphores.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Set up stderr
 *
 * @return int 0 on success, -1 on failure
 */
int init_stderr();
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
/**
 * @brief Wether or not stderr is empty
 *
 * @return true stderr is empty
 * @return false stderr is not empty
 */
bool stderr_empty();
/**
 * @brief Flush stderr
 */
void flush_stderr();

#endif
