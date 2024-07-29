#include "network.h"
#include "arp.h"
#include "ip.h"

#include "../physical/rtl8139.h"

#include <stdio.h>

static uint8_t broadcast_mac_address[MAC_LENGTH] = {
    BROADCAST_MAC_PARTITION,
    BROADCAST_MAC_PARTITION,
    BROADCAST_MAC_PARTITION,
    BROADCAST_MAC_PARTITION,
    BROADCAST_MAC_PARTITION,
    BROADCAST_MAC_PARTITION,
};

void receive(EthernetFrame *packet, int length)
{
    void *data = (void *)packet + sizeof(EthernetFrame);
    int data_length = length - sizeof(EthernetFrame);

    int type = endian_word(packet->type);

    switch (type)
    {
    case ETHERNET_TYPE_IPV4:
        // IPv4
        ip_handle_packet(data, data_length);
        break;
    case ETHERNET_TYPE_ARP:
        // ARP
        arp_handle_packet(data, data_length);
        break;
    default:
        // Unknown
        break;
    }
}

int transmit(uint8_t *dest, const void *data, int length, uint16_t protocol)
{
    EthernetFrame *frame = malloc(sizeof(EthernetFrame) + length);
    void *frame_data = (void *)frame + sizeof(EthernetFrame);

    if (!dest)
    {
        dest = broadcast_mac_address;
    }

    get_MAC_address(frame->src);
    memcpy(frame->dest, dest, MAC_LENGTH);
    frame->type = endian_word(protocol);
    memcpy(frame_data, data, length);

    ncPrint("Transmitting packet to ");
    char mac_str[20];
    snprintf(mac_str, 20, "%02x:%02x:%02x:%02x:%02x:%02x", frame->dest[0], frame->dest[1], frame->dest[2], frame->dest[3], frame->dest[4], frame->dest[5]);
    ncPrint(mac_str);
    ncNewline();

    int sent = rtl8139_send_packet(frame, sizeof(EthernetFrame) + length);

    free(frame);

    return sent;
}

static uint8_t flip_byte(uint8_t byte, int num_bits)
{
    uint8_t t = byte << (8 - num_bits);
    return t | (byte >> num_bits);
}

uint8_t ntohb(uint8_t byte, int num_bits)
{
    return flip_byte(byte, 8 - num_bits);
}

uint8_t htonb(uint8_t byte, int num_bits)
{
    return flip_byte(byte, num_bits);
}
