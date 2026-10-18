#ifndef PETHNIP_H
#define PETHNIP_H

#include "network.h"

#define IP_IPV4 4

#define IP_PACKET_NO_FRAGMENT 2
#define IP_IS_LAST_FRAGMENT 4

typedef struct IPPacket
{
    char version_ihl_ptr[0];
    uint8_t version : 4;
    uint8_t ihl : 4;
    uint8_t tos;
    uint16_t length;
    uint16_t id;
    char flags_fragment_ptr[0];
    uint8_t flags : 3;
    uint8_t fragment_offset_high : 5;
    uint8_t fragment_offset_low;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    uint8_t src_ip[IPV4_LENGTH];
    uint8_t dst_ip[IPV4_LENGTH];
    uint8_t data[];
} __attribute__((packed)) IPPacket;

/**
 * @brief Handle an IP packet (Network layer)
 * 
 * @param packet The IP packet
 * @param length The length of the packet
 */
void ip_handle_packet(IPPacket *packet, int length);
/**
 * @brief Send an IP packet (Network layer)
 * 
 * @param dst_ip The destination IP address
 * @param data The data to send
 * @param length The length of the data
 * @param protocol The protocol of the packet
 * @return int The number of bytes sent, 0 if the transmission was sent to another thread, -1 if request failed
 */
int ip_send_packet(uint8_t *dst_ip, void *data, int length, uint8_t protocol);

#endif
