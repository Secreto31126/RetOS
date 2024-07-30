#ifndef PETHUDP_H
#define PETHUDP_H

#include "transport.h"

typedef struct UDPPacket {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
    uint8_t data[];
} __attribute__((packed)) UDPPacket;

/**
 * @brief Send a UDP packet (Transport layer)
 * 
 * @param dst_ip The destination IP address
 * @param src_port The source port
 * @param dst_port The destination port
 * @param data The data to send
 * @param len The length of the data
 * @return int The number of bytes sent, -1 if request failed
 */
int udp_send_packet(uint8_t dst_ip[IPV4_LENGTH], uint16_t src_port, uint16_t dst_port, void *data, int len);
/**
 * @brief Handle UDP packet (Transport layer)
 * 
 * @param packet The received packet
 * @param length The length of the packet
 * @param src_ip The source IP address
 * @param dst_ip The destination IP address
 */
void udp_handle_packet(UDPPacket *packet, uint16_t length, uint8_t src_ip[IPV4_LENGTH], uint8_t dst_ip[IPV4_LENGTH]);

#endif
