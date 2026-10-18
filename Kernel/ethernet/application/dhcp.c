#include "dhcp.h"

#include "../transport/udp.h"

#include <stdio.h>

/**
 * @brief Search for the value of a type in options
 */
static void *get_dhcp_options(DHCPPacket *packet, uint8_t type, const uint8_t *end);
static void make_dhcp_packet(DHCPPacket *packet, DHCPMessageType msg_type, uint8_t *request_ip);

static char ip_addr[IPV4_LENGTH] = {0};
static bool is_ip_allocated;

bool gethostaddr(uint8_t addr[IPV4_LENGTH])
{
    memcpy(addr, ip_addr, IPV4_LENGTH);
    return is_ip_allocated;
}

int dhcp_discover()
{
    uint8_t request_ip[IPV4_LENGTH];
    uint8_t dst_ip[IPV4_LENGTH];

    memset(request_ip, 0, IPV4_LENGTH);
    memset(dst_ip, BROADCAST_IP_PARTITION, IPV4_LENGTH);

    // calloc
    DHCPPacket *packet = malloc(sizeof(DHCPPacket));
    memset(packet, 0, sizeof(DHCPPacket));

    make_dhcp_packet(packet, DHCP_DISCOVER, request_ip);
    int sent = udp_send_packet(dst_ip, 68, 67, packet, sizeof(DHCPPacket));

    free(packet);

    return sent;
}

int dhcp_request(uint8_t *request_ip)
{
    uint8_t dst_ip[IPV4_LENGTH];
    memset(dst_ip, BROADCAST_MAC_PARTITION, IPV4_LENGTH);

    // calloc
    DHCPPacket *packet = malloc(sizeof(DHCPPacket));
    memset(packet, 0, sizeof(DHCPPacket));

    make_dhcp_packet(packet, DHCP_REQUEST, request_ip);
    int sent = udp_send_packet(dst_ip, 68, 67, packet, sizeof(DHCPPacket));

    free(packet);

    return sent;
}

void dhcp_handle_packet(DHCPPacket *packet, uint16_t length)
{
    // Fixed header and magic cookie
    if (length < offsetof(DHCPPacket, options) + 4 || packet->xid != endian_dword(DHCP_TRANSACTION_IDENTIFIER))
    {
        return;
    }

    if (packet->op == DHCP_REPLY_OPERATION)
    {
        uint8_t *type = get_dhcp_options(packet, 53, (uint8_t *)packet + length);

        if (!type)
        {
            return;
        }

        if (*type == DHCP_OFFER)
        {
            // Offer, return a request
            ncPrint("Received DHCP Offer: ");
            char ip_str[20];
            get_ip_str(ip_str, (uint8_t *)&packet->your_ip);
            ncPrint(ip_str);
            ncNewline();

            dhcp_request((uint8_t *)&packet->your_ip);
        }
        else if (*type == DHCP_ACK)
        {
            // ACK, save necessary info (IP for example)
            memcpy(ip_addr, &packet->your_ip, IPV4_LENGTH);
            is_ip_allocated = true;

            // ncPrint("Hello, I'm ");
            // char name[64];
            // snprintf(name, 64, "%s", packet->server_name);
            // ncPrint(name);
            // ncPrint(", your DHCP server for today\n");
        }

        free(type);
    }
}

static void *get_dhcp_options(DHCPPacket *packet, uint8_t type, const uint8_t *end)
{
    uint8_t *options = packet->options + 4;

    while (options < end && *options != 0xFF)
    {
        uint8_t curr_type = *options;

        // Pad option, no length byte
        if (curr_type == 0)
        {
            options++;
            continue;
        }

        if (options + 2 > end)
        {
            break;
        }

        uint8_t len = *(options + 1);

        if (options + 2 + len > end)
        {
            break;
        }

        if (curr_type == type)
        {
            if (!len)
            {
                return NULL;
            }

            // Found type, return value
            void *ret = malloc(len);
            if (ret)
            {
                memcpy(ret, options + 2, len);
            }

            return ret;
        }

        options += (2 + len);
    }

    return NULL;
}

static void make_dhcp_packet(DHCPPacket *packet, DHCPMessageType msg_type, uint8_t *request_ip)
{
    packet->op = DHCP_REQUEST_OPERATION;
    packet->hardware_type = HARDWARE_TYPE_ETHERNET;
    packet->hardware_addr_len = MAC_LENGTH;
    packet->hops = 0;
    packet->xid = endian_dword(DHCP_TRANSACTION_IDENTIFIER);
    packet->flags = endian_word(0x8000);
    get_MAC_address(packet->client_hardware_addr);

    // Options specific to DHCP Discover (required)

    // Magic Cookie
    uint8_t *options = packet->options;
    *((uint32_t *)(options)) = endian_dword(0x63825363);
    options += 4;

    // First option, message type = DHCP_DISCOVER/DHCP_REQUEST
    *(options++) = 53;
    *(options++) = 1;
    *(options++) = msg_type;

    // Client identifier
    *(options++) = 61;
    *(options++) = 0x07;
    *(options++) = 0x01;
    get_MAC_address(options);
    options += MAC_LENGTH;

    // Requested IP address
    *(options++) = 50;
    *(options++) = 0x04;
    memcpy((uint32_t *)(options), request_ip, IPV4_LENGTH);
    options += 4;

    // Host Name
    *(options++) = 12;
    *(options++) = strlen("retos");
    memcpy(options, "retos", strlen("retos"));
    options += strlen("retos");

    // Parameter request list
    *(options++) = 55;
    *(options++) = 8;
    *(options++) = 0x1;
    *(options++) = 0x3;
    *(options++) = 0x6;
    *(options++) = 0xf;
    *(options++) = 0x2c;
    *(options++) = 0x2e;
    *(options++) = 0x2f;
    *(options++) = 0x39;
    *(options++) = 0xff;
}
