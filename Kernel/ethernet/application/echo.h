#ifndef PETHTLP_H
#define PETHTLP_H

#include "application.h"
#include "../transport/transport.h"

/**
 * @brief Broadcast a ECHO request (Application layer)
 *
 * @param data The request data
 * @param length The length of the request
 * @param dst_ip The destination IP address
 * @param src_port The source port
 * @param dst_port The destination port
 * @return int The number of bytes sent, -1 if request failed
 */
int echo_request(void *data, int length, uint8_t dst_ip[IPV4_LENGTH], uint16_t src_port, uint16_t dst_port);
/**
 * @brief Handle ECHO offer packet (Application layer)
 *
 * @param packet The ECHO packet
 * @param length The length of the packet
 * @param src_ip The source IP address
 * @param dst_ip The destination IP address
 * @param src_port The source port
 * @param dst_port The destination port
 */
void echo_handle_packet(void *packet, uint16_t length, uint8_t src_ip[IPV4_LENGTH], uint8_t dst_ip[IPV4_LENGTH], uint16_t src_port, uint16_t dst_port);

#endif
