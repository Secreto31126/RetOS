#include "udp.h"

#include "../network/ip.h"

static uint16_t udp_calculate_checksum(UDPPacket *packet);

int udp_send_packet(uint8_t dst_ip[IPV4_LENGTH], uint16_t src_port, uint16_t dst_port, void *data, int length)
{
    UDPPacket *packet = malloc(sizeof(UDPPacket) + length);
    memset(packet, 0, sizeof(UDPPacket));

    packet->src_port = endian_word(src_port);
    packet->dst_port = endian_word(dst_port);
    packet->length = endian_word(sizeof(UDPPacket) + length);
    packet->checksum = 0;

    memcpy(packet->data, data, length);

    // Calculate checksum
    packet->checksum = udp_calculate_checksum(packet);

    int sent = ip_send_packet(dst_ip, packet, sizeof(UDPPacket) + length, PROTOCOL_UDP);

    free(packet);

    return sent;
}

void udp_handle_packet(UDPPacket *packet, uint16_t length, uint8_t src_ip[IPV4_LENGTH], uint8_t dst_ip[IPV4_LENGTH])
{
    uint16_t src_port = endian_word(packet->src_port);
    uint16_t dst_port = endian_word(packet->dst_port);

    void *data_ptr = packet->data;
    uint16_t data_length = endian_word(packet->length) - sizeof(UDPPacket);

    Port port = Ports[dst_port];
    if ((port.protocol & PROTOCOL_UDP) == PROTOCOL_UDP && port.handler != NULL)
    {
        port.handler(data_ptr, data_length, src_ip, dst_ip, src_port, dst_port);
    }
}

static uint16_t udp_calculate_checksum(UDPPacket *packet)
{
    // UDP checksum is optional in IPv4
    return 0;
}
