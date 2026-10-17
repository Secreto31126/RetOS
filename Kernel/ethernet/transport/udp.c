#include "udp.h"

#include "../network/ip.h"

static uint16_t udp_calculate_checksum(UDPPacket *packet, uint16_t length, uint8_t src_ip[IPV4_LENGTH], uint8_t dst_ip[IPV4_LENGTH]);

int udp_send_packet(uint8_t dst_ip[IPV4_LENGTH], uint16_t src_port, uint16_t dst_port, void *data, int length)
{
    UDPPacket *packet = malloc(sizeof(UDPPacket) + length);
    if (!packet)
    {
        return -1;
    }

    memset(packet, 0, sizeof(UDPPacket));

    uint16_t udp_length = sizeof(UDPPacket) + length;

    packet->src_port = endian_word(src_port);
    packet->dst_port = endian_word(dst_port);
    packet->length = endian_word(udp_length);
    packet->checksum = 0;

    memcpy(packet->data, data, length);

    uint8_t src_ip[IPV4_LENGTH];
    gethostaddr(src_ip);

    // A zero checksum means "no checksum", so send its one's complement equivalent
    uint16_t checksum = udp_calculate_checksum(packet, udp_length, src_ip, dst_ip);
    packet->checksum = endian_word(checksum ? checksum : 0xFFFF);

    int sent = ip_send_packet(dst_ip, packet, udp_length, PROTOCOL_UDP);

    free(packet);

    return sent;
}

void udp_handle_packet(UDPPacket *packet, uint16_t length, uint8_t src_ip[IPV4_LENGTH], uint8_t dst_ip[IPV4_LENGTH])
{
    if (length < sizeof(UDPPacket))
    {
        return;
    }

    uint16_t udp_length = endian_word(packet->length);
    if (udp_length < sizeof(UDPPacket) || udp_length > length)
    {
        return;
    }

    if (packet->checksum && udp_calculate_checksum(packet, udp_length, src_ip, dst_ip))
    {
        return;
    }

    uint16_t src_port = endian_word(packet->src_port);
    uint16_t dst_port = endian_word(packet->dst_port);

    void *data_ptr = packet->data;
    uint16_t data_length = udp_length - sizeof(UDPPacket);

    Port port = Ports[dst_port];
    if (port.protocol == PROTOCOL_UDP && port.handler != NULL)
    {
        port.handler(data_ptr, data_length, src_ip, dst_ip, src_port, dst_port);
    }
}

static uint16_t udp_calculate_checksum(UDPPacket *packet, uint16_t length, uint8_t src_ip[IPV4_LENGTH], uint8_t dst_ip[IPV4_LENGTH])
{
    uint8_t pseudo_header[12];
    memcpy(pseudo_header, src_ip, IPV4_LENGTH);
    memcpy(pseudo_header + IPV4_LENGTH, dst_ip, IPV4_LENGTH);
    pseudo_header[8] = 0;
    pseudo_header[9] = PROTOCOL_UDP;
    pseudo_header[10] = length >> 8;
    pseudo_header[11] = length & 0xFF;

    uint16_t pseudo_header_sum = ~inet_checksum(pseudo_header, sizeof(pseudo_header), 0);
    return inet_checksum(packet, length, pseudo_header_sum);
}
