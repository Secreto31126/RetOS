#ifndef ETH_H
#define ETH_H

#include <stdint.h>
#include <stdbool.h>

#define MAC_LENGTH 6  // MAC address length
#define IPV4_LENGTH 4 // IP address length

/**
 * @brief Initializes the ethernet module
 *
 * @return uint64_t The MAC address
 */
uint64_t init_ethernet();
/**
 * @brief Handles an ethernet interruption
 */
void ethernet_manager();
/**
 * @brief Queue a received frame for the network daemon, dropped if the queue is full
 *
 * @param frame The ethernet frame, without CRC
 * @param length The length of the frame
 */
void ethernet_enqueue(const void *frame, uint16_t length);
/**
 * @brief Process queued frames forever, run as a kernel daemon
 */
void ethernet_daemon();
/**
 * @brief Process every queued frame without blocking
 */
void ethernet_poll();

/**
 * @brief Get the MAC address object
 *
 * @param mac The MAC address will be stored here
 */
void get_MAC_address(uint8_t mac[MAC_LENGTH]);
/**
 * @brief Get the MAC address
 *
 * @return uint64_t The MAC address
 */
uint64_t get_MAC_address_64();

/**
 * @brief Request a new IP address from the DHCP server
 * 
 * @return int The number of bytes sent, -1 if request failed
 */
int dhcp_discover();
/**
 * @brief Retrieve the IP address of the host
 * 
 * @param addr The IP address will be stored here
 * @return true The IP address is allocated
 * @return false The IP address was not yet allocated via DHCP, addr content is invalid
 */
bool gethostaddr(uint8_t addr[IPV4_LENGTH]);

#endif
