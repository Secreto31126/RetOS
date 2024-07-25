#include "echo.h"

#include "../transport/udp.h"

#include <stdio.h>

int echo_request(void *data, int length, uint8_t dst_ip[IPV4_LENGTH], uint16_t src_port, uint16_t dst_port)
{
    void *packet = malloc(length);
    memcpy(packet, data, length);

    int sent = udp_send_packet(dst_ip, src_port, dst_port, packet, length);

    free(packet);

    return sent;
}

void echo_handle_packet(void *packet, uint16_t length, uint8_t src_ip[IPV4_LENGTH], uint8_t dst_ip[IPV4_LENGTH], uint8_t src_port, uint8_t dst_port)
{
    ncPrint("Received ECHO packet from ");

    char src_ip_str[20];
    get_ip_str(src_ip_str, src_ip);
    ncPrint(src_ip_str);

    ncPrint(":");
    ncPrintDec(src_port);
    ncPrint(" - ");

    char *data = malloc(length + 1);
    snprintf(data, length, "%s", packet);
    ncPrint(data);

    udp_send_packet(src_ip, dst_port, src_port, packet, length);

    free(data);
}
