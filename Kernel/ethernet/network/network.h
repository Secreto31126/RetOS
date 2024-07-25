#ifndef PETHNET_H
#define PETHNET_H

#include "../ethernet.h"
#include "../physical/physical.h"
#include "../transport/transport.h"

#define PROTOCOL_UDP 17 // 0b00010001
#define PROTOCOL_TCP 6  // 0b00000110

#define BROADCAST_MAC_PARTITION 0xFF // Broadcast MAC value
#define BROADCAST_IP_PARTITION 0xFF // Broadcast IP value

typedef struct EthernetFrame
{
    uint8_t dest[MAC_LENGTH];
    uint8_t src[MAC_LENGTH];
    uint16_t type;
    uint8_t data[];
} __attribute__((packed)) EthernetFrame;

/**
 * @brief Receive a packet (Network layer)
 * 
 * @param packet The packet to receive
 * @param length The length of the packet
 */
void receive(EthernetFrame *packet, int length);
/**
 * @brief Send a packet (Network layer)
 *
 * @param dest The destination MAC address (if NULL, broadcast)
 * @param data The content of the packet
 * @param length The length of the packet
 * @param protocol The protocol of the packet (little endian)
 * @return int The number of bytes sent
 */
int transmit(uint8_t *dest, const void *data, int length, uint16_t protocol);

void init_arp();

uint8_t ntohb(uint8_t byte, int num_bits);
uint8_t htonb(uint8_t byte, int num_bits);

#endif
