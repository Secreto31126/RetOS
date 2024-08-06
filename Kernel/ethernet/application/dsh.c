#include "echo.h"

#include "../transport/udp.h"

#include <stdin.h>
#include <stdio.h>

int dsh_request(void *data, int length, uint8_t dst_ip[IPV4_LENGTH], uint16_t src_port, uint16_t dst_port)
{
    void *packet = malloc(length);
    memcpy(packet, data, length);

    int sent = udp_send_packet(dst_ip, src_port, dst_port, packet, length);

    free(packet);

    return sent;
}

void dsh_handle_packet(void *packet, uint16_t length, uint8_t src_ip[IPV4_LENGTH], uint8_t dst_ip[IPV4_LENGTH], uint16_t src_port, uint16_t dst_port)
{
    write_stdin(packet, length);

    if (src_port)
    {
        udp_send_packet(src_ip, dst_port, src_port, "OK\n", sizeof("OK\n") - 1);
    }
}
