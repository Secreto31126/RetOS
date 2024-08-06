#ifndef PETHRTL_H
#define PETHRTL_H

#include "physical.h"
#include "../network/network.h"

#define MAX_CONCURRENT_PACKETS 4

/**
 * @brief Initializes the RTL8139 module
 */
void init_rtl8139();
/**
 * @brief Get the MAC address object
 *
 * @param mac The MAC address will be stored here
 */
void rtl8139_get_MAC_address(uint8_t mac[MAC_LENGTH]);
/**
 * @brief Send a packet (Physical layer)
 *
 * @param data The data to send
 * @param length The length of the data
 * @return int The number of bytes sent, -1 if the buffer is full
 */
int rtl8139_send_packet(void *data, uint32_t len);
/**
 * @brief Receive a packet (Physical layer)
 */
void rtl8139_receive_packet();
/**
 * @brief Handles an RTL8139 interruption
 */
void rtl8139_handle_interrupt();

#endif
