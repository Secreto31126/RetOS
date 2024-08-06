#include "ip.h"

#include "arp.h"

#include "../transport/udp.h"
#include "../transport/tcp.h"

#include <stdio.h>
#include <unistd.h>

static int delayed_ip_send_packet(IPPacket *delayed_packet);
static uint16_t ip_calculate_checksum(IPPacket *packet);

void ip_handle_packet(IPPacket *packet, int length)
{
    // Fix packet data order (be careful with the endiness problem within a byte)
    *((uint8_t *)(&packet->version_ihl_ptr)) = ntohb(*((uint8_t *)(&packet->version_ihl_ptr)), 4);
    *((uint8_t *)(packet->flags_fragment_ptr)) = ntohb(*((uint8_t *)(packet->flags_fragment_ptr)), 3);

    // Dump source ip, data, checksum
    if (packet->version == IP_IPV4)
    {
        void *data_ptr = (void *)packet + packet->ihl * 4;
        int data_length = endian_word(packet->length) - sizeof(IPPacket);

        ncPrint("Received IP packet from ");

        char src_ip[20];
        get_ip_str(src_ip, packet->src_ip);
        ncPrint(src_ip);

        ncPrint(" to ");

        char dst_ip[20];
        get_ip_str(dst_ip, packet->dst_ip);
        ncPrint(dst_ip);

        ncPrint(" with protocol ");

        switch (packet->protocol)
        {
        case PROTOCOL_UDP:
            ncPrint("UDP\n");
            udp_handle_packet(data_ptr, data_length, packet->src_ip, packet->dst_ip);
            break;

        case PROTOCOL_TCP:
            ncPrint("TCP\n");
            // tcp_handle_packet(data_ptr, data_length, packet->src_ip, packet->dst_ip);
            break;

        default:
            ncPrint("Unknown\n");
            break;
        }
    }
}

int ip_send_packet(uint8_t *dst_ip, void *data, int length, uint8_t protocol)
{
    int packet_length = sizeof(IPPacket) + length;

    IPPacket *packet = malloc(packet_length);
    memset(packet, 0, sizeof(IPPacket));

    packet->version = IP_IPV4;
    // No options
    packet->ihl = 5;
    // Don't care, set to 0
    packet->tos = 0;
    packet->length = endian_word(packet_length);
    // Used for ip fragmentation, don't care now
    packet->id = 0;
    // Tell router to not divide the packet, and this is packet is the last piece of the fragments.
    packet->flags = 0;
    packet->fragment_offset_high = 0;
    packet->fragment_offset_low = 0;

    packet->ttl = 255;
    packet->protocol = protocol;

    gethostaddr(packet->src_ip);
    memcpy(packet->dst_ip, dst_ip, IPV4_LENGTH);

    void *packet_data = packet->data;
    memcpy(packet_data, data, length);

    // Fix packet data order
    *((uint8_t *)(&packet->version_ihl_ptr)) = htonb(*((uint8_t *)(&packet->version_ihl_ptr)), 4);
    *((uint8_t *)(packet->flags_fragment_ptr)) = htonb(*((uint8_t *)(packet->flags_fragment_ptr)), 3);

    // Make sure checksum is 0 before checksum calculation
    packet->header_checksum = 0;
    packet->header_checksum = endian_word(ip_calculate_checksum(packet));

    // packet->header_checksum = endian_word(cksum(packet));
    //  Don't care to pad, because we don't use the option field in ip packet
    /*
     * If the ip is in the same network, the destination mac address is the routers's mac address, the router'll figure out how to route the packet
     * Now, again, let's assume it's always in the same network, because i want to test if the simplest ip packet sending works as i write the code
     * */

    uint8_t dst_hardware_addr[MAC_LENGTH];

    // If the destination mac address is not in the arp table, reschedule the packet sending to another thread
    if (!arp_lookup(dst_hardware_addr, dst_ip))
    {
        return add_task((Callback)delayed_ip_send_packet, (void *)packet);
    }

    // Got the mac address! Now send an ethernet packet
    int sent = transmit(dst_hardware_addr, packet, packet_length, ETHERNET_TYPE_IPV4);

    free(packet);

    return sent;
}

static int delayed_ip_send_packet(IPPacket *packet)
{
    uint8_t dst_ip[IPV4_LENGTH];
    memcpy(dst_ip, packet->dst_ip, IPV4_LENGTH);

    int arp_sent = 3;
    while (arp_sent--)
    {
        // Send an arp request
        arp_send_packet(dst_ip);

        // Wait for 0.25 second
        usleep(5);

        uint8_t dst_hardware_addr[MAC_LENGTH];
        if (arp_lookup(dst_hardware_addr, dst_ip))
        {
            // Got the mac address! Now send an ethernet packet
            transmit(dst_hardware_addr, packet, endian_word(packet->length), ETHERNET_TYPE_IPV4);
            break;
        }

        if (!arp_sent)
        {
            ncPrint("ARP request failed\n");
            transmit(NULL, packet, endian_word(packet->length), ETHERNET_TYPE_IPV4);
        }
    }

    free(packet);
    return 0;
}

int get_ip_str(char *ip_str, uint8_t *ip)
{
    return snprintf(ip_str, 20, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

static uint16_t ip_calculate_checksum(IPPacket *packet)
{
    // Treat the packet header as a 2-byte-integer array
    // Sum all integers up and flip all bits
    int array_size = sizeof(IPPacket) / 2;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
    uint16_t *array = (uint16_t *)packet;
#pragma GCC diagnostic pop

    uint32_t sum = 0;
    for (int i = 0; i < array_size; i++)
    {
        sum += endian_word(array[i]);
    }

    uint32_t carry = sum >> 16;

    sum = sum & 0x0000ffff;
    sum = sum + carry;

    uint16_t ret = ~sum;

    return ret;
}
