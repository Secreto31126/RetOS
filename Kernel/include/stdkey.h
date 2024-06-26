#ifndef STDK_H
#define STDK_H

#include <semaphores.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Set up stdkey
 *
 * @return int 0 on success, -1 on failure
 */
int init_stdkey();
/**
 * @brief Read from stdkey
 *
 * @param buf The direction to store the data
 * @param len The number of bytes to read
 * @return uint16_t The number of bytes read
 */
uint16_t read_stdkey(uint8_t *buf, uint16_t len);
/**
 * @brief Write to stdkey
 *
 * @param buf The data to write
 * @param len The number of bytes to write
 * @return uint16_t The number of bytes written
 */
uint16_t write_stdkey(uint8_t *buf, uint16_t len);
/**
 * @brief Handle a key event
 *
 * @param scancode The scancode of the key
 */
void stdkey_keyevent(uint8_t scancode);
/**
 * @brief Wether or not stdkey is empty
 *
 * @return true stdkey is empty
 * @return false stdkey is not empty
 */
bool stdkey_empty();
/**
 * @brief Flush stdkey
 */
void flush_stdkey();

#endif
