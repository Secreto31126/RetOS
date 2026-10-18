#ifndef PETHARP_H
#define PETHARP_H

#include "network.h"

typedef struct ARPPacket
{
    uint16_t hardware_type;
    uint16_t protocol;
    uint8_t hardware_addr_len;
    uint8_t protocol_addr_len;
    uint16_t opcode;
    uint8_t src_hardware_addr[MAC_LENGTH];
    uint8_t src_protocol_addr[IPV4_LENGTH];
    uint8_t dst_hardware_addr[MAC_LENGTH];
    uint8_t dst_protocol_addr[IPV4_LENGTH];
} __attribute__((packed)) ARPPacket;

typedef struct ARPTableEntry
{
    uint32_t ip_addr;
    uint64_t mac_addr;
} ARPTableEntry;

/**
 * @brief Handle an ARP packet (Network layer)
 * 
 * @param arp_packet The ARP packet
 * @param length The length of the packet
 */
void arp_handle_packet(ARPPacket *arp_packet, int length);
/**
 * @brief Send an ARP packet (Network layer)
 *
 * @param dst_protocol_addr The destination protocol address
 * @return int The number of bytes sent
 */
int arp_send_packet(uint8_t dst_protocol_addr[IPV4_LENGTH]);
/**
 * @brief Search for an ARP entry
 * 
 * @param mac_addr Where to store the MAC address
 * @param ip_addr The IP address to search for
 * @return true The entry was found
 * @return false The entry was not found
 */
bool arp_lookup(uint8_t mac_addr[MAC_LENGTH], uint8_t ip_addr[IPV4_LENGTH]);

#endif
