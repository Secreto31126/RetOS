#ifndef PETHDHC_H
#define PETHDHC_H

#include "application.h"
#include "../transport/transport.h"

#define DHCP_REQUEST_OPERATION 1
#define DHCP_REPLY_OPERATION 2

typedef enum DHCPMessageType
{
    DHCP_DISCOVER = 1,
    DHCP_OFFER = 2,
    DHCP_REQUEST = 3,
    DHCP_DECLINE = 4,
    DHCP_ACK = 5,
    DHCP_NAK = 6,
    DHCP_RELEASE = 7,
    DHCP_INFORM = 8
} DHCPMessageType;

#define DHCP_TRANSACTION_IDENTIFIER 0x55555555

typedef struct DHCPPacket
{
    uint8_t op;
    uint8_t hardware_type;
    uint8_t hardware_addr_len;
    uint8_t hops;
    uint32_t xid;
    uint16_t seconds;
    uint16_t flags;     // may be broken
    uint32_t client_ip; //
    uint32_t your_ip;
    uint32_t server_ip;
    uint32_t gateway_ip;
    uint8_t client_hardware_addr[16];
    uint8_t server_name[64];
    uint8_t file[128];
    uint8_t options[64];
} __attribute__((packed)) DHCPPacket;

/**
 * @brief Broadcast a DHCP request (Application layer)
 * 
 * @param request_ip The requested IP
 * @return int The number of bytes sent, -1 if request failed
 */
int dhcp_request(uint8_t *request_ip);
/**
 * @brief Handle DHCP offer packet (Application layer)
 * 
 * @param packet The DHCP packet
 * @param length The length of the packet
 */
void dhcp_handle_packet(DHCPPacket *packet, uint16_t length);

#endif
