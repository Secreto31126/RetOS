#include "arp.h"

#include <stdio.h>

#define ARP_REQUEST 1
#define ARP_REPLY 2

#define ARP_TABLE_SIZE 512

/**
 * @brief Add an ARP entry
 *
 * @param mac_addr The MAC address
 * @param ip_addr The IP address
 */
static void arp_lookup_upsert(uint8_t *mac_addr, uint8_t *ip_addr);

static ARPTableEntry arp_table[ARP_TABLE_SIZE];
static signed int arp_table_size = 0;
static signed int arp_table_curr = 0;

void arp_handle_packet(ARPPacket *packet, int length)
{
    uint8_t dst_mac_addr[MAC_LENGTH];
    uint8_t dst_ip_addr[IPV4_LENGTH];

    // Save some packet field
    memcpy(dst_mac_addr, packet->src_hardware_addr, MAC_LENGTH);
    memcpy(dst_ip_addr, packet->src_protocol_addr, IPV4_LENGTH);

    if (endian_word(packet->hardware_type) != HARDWARE_TYPE_ETHERNET || endian_word(packet->protocol) != ETHERNET_TYPE_IPV4)
    {
        return;
    }

    char ip[IPV4_LENGTH * 4 + 1];
    get_ip_str(ip, packet->dst_protocol_addr);
    ncPrint(ip);

    ncPrint(": ARP ");

    // Store the ip-mac address mapping relation
    arp_lookup_upsert(dst_mac_addr, dst_ip_addr);

    // Reply arp request, if the ip address matches
    if (endian_word(packet->opcode) == ARP_REQUEST)
    {
        ncPrint("REQUEST\n");

        uint8_t host_ip[IPV4_LENGTH];
        if (!gethostaddr(host_ip)) {
            return;
        }

        if (memcmp(packet->dst_protocol_addr, host_ip, IPV4_LENGTH))
        {
            // Set source MAC address, IP address
            get_MAC_address(packet->src_hardware_addr);
            memcpy(packet->src_protocol_addr, host_ip, IPV4_LENGTH);

            // Set destination MAC address, IP address
            memcpy(packet->dst_hardware_addr, dst_mac_addr, MAC_LENGTH);
            memcpy(packet->dst_protocol_addr, dst_ip_addr, IPV4_LENGTH);

            // Set opcode
            packet->opcode = endian_word(ARP_REPLY);

            // Set lengths
            packet->hardware_addr_len = MAC_LENGTH;
            packet->protocol_addr_len = IPV4_LENGTH;

            // Set hardware type
            packet->hardware_type = endian_word(HARDWARE_TYPE_ETHERNET);

            // Set protocol = IPv4
            packet->protocol = endian_word(ETHERNET_TYPE_IPV4);

            // Now send it with ethernet
            int sent = transmit(dst_mac_addr, (void *)packet, sizeof(ARPPacket), ETHERNET_TYPE_ARP);

            if (sent < 0)
            {
                ncPrint("ARP reply failed\n");
            }
        }
    }
    else if (endian_word(packet->opcode) == ARP_REPLY)
    {
        // May be we can handle the case where we get a reply after sending a request, but i don't think my os will ever need to do so...
        // But RetOS will
        ncPrint("REPLY\n");

        char mac[MAC_LENGTH * 4 + 1];
        snprintf(mac, MAC_LENGTH * 4 + 1, "%02x:%02x:%02x:%02x:%02x:%02x", dst_mac_addr[0], dst_mac_addr[1], dst_mac_addr[2], dst_mac_addr[3], dst_mac_addr[4], dst_mac_addr[5]);
        ncPrint("\tMAC: ");
        ncPrint(mac);
        ncNewline();
    }
    else
    {
        ncNewline();
        // qemu_printf("Got unknown ARP, opcode = %d\n", packet->opcode);
    }
}

int arp_send_packet(uint8_t dst_protocol_addr[IPV4_LENGTH])
{
    ARPPacket *packet = malloc(sizeof(ARPPacket));

    // Set source MAC address, IP address
    get_MAC_address(packet->src_hardware_addr);
    gethostaddr(packet->src_protocol_addr);

    // Set destination MAC address (ignored in requests), IP address
    memset(packet->dst_hardware_addr, BROADCAST_MAC_PARTITION, MAC_LENGTH);
    memcpy(packet->dst_protocol_addr, dst_protocol_addr, IPV4_LENGTH);

    // Set opcode
    packet->opcode = endian_word(ARP_REQUEST);

    // Set lengths
    packet->hardware_addr_len = MAC_LENGTH;
    packet->protocol_addr_len = IPV4_LENGTH;

    // Set hardware type
    packet->hardware_type = endian_word(HARDWARE_TYPE_ETHERNET);

    // Set protocol = IPv4
    packet->protocol = endian_word(ETHERNET_TYPE_IPV4);

    // Now broadcast it with ethernet
    int sent = transmit(NULL, (uint8_t *)packet, sizeof(ARPPacket), ETHERNET_TYPE_ARP);

    free(packet);

    return sent;
}

static void arp_lookup_upsert(uint8_t *mac_addr, uint8_t *ip_addr)
{
    // memcpy(&arp_table[arp_table_curr].mac_addr, mac_addr, MAC_LENGTH);
    // memcpy(&arp_table[arp_table_curr].ip_addr, ip_addr, IPV4_LENGTH);

    // if (arp_table_size < ARP_TABLE_SIZE)
    // {
    //     arp_table_size++;
    // }

    // // Wrap around
    // if (arp_table_curr++ >= ARP_TABLE_SIZE)
    // {
    //     arp_table_curr = 0;
    // }

    for (int i = 0; i < arp_table_size; i++)
    {
        if (arp_table[i].ip_addr == *((uint32_t *)ip_addr))
        {
            memcpy(&arp_table[i].mac_addr, mac_addr, MAC_LENGTH);
            return;
        }
    }

    if (arp_table_size < ARP_TABLE_SIZE)
    {
        arp_table_size++;
    }

    memcpy(&arp_table[arp_table_curr].mac_addr, mac_addr, MAC_LENGTH);
    memcpy(&arp_table[arp_table_curr].ip_addr, ip_addr, IPV4_LENGTH);

    // Wrap around
    if (arp_table_curr++ >= ARP_TABLE_SIZE)
    {
        arp_table_curr = 0;
    }
}

bool arp_lookup(uint8_t mac_addr[MAC_LENGTH], uint8_t ip_addr[IPV4_LENGTH])
{
    uint32_t ip = *((uint32_t *)(ip_addr));
    for (int i = 0; i < arp_table_size; i++)
    {
        if (arp_table[i].ip_addr == ip)
        {
            memcpy(mac_addr, &arp_table[i].mac_addr, MAC_LENGTH);
            return true;
        }
    }

    return false;
}

void init_arp()
{
    uint8_t broadcast_mac[MAC_LENGTH];
    uint8_t broadcast_ip[IPV4_LENGTH];

    memset(broadcast_mac, BROADCAST_MAC_PARTITION, MAC_LENGTH);
    memset(broadcast_ip, BROADCAST_IP_PARTITION, IPV4_LENGTH);

    arp_lookup_upsert(broadcast_mac, broadcast_ip);
}
