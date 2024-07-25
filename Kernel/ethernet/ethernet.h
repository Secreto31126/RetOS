#ifndef PETH_H
#define PETH_H

#include <ethernet.h>
#include <lib.h>
#include <console.h>
#include <mman.h>
#include <string.h>
#include <tasker.h>

/**
 * @brief A handler for a port (UDP or TCP)
 */
typedef void (*PORT_HANDLER)(void *data, uint16_t length, uint8_t src_ip[IPV4_LENGTH], uint8_t dst_ip[IPV4_LENGTH], uint16_t src_port, uint16_t dst_port);

typedef struct Port {
    uint16_t protocol;
    PORT_HANDLER handler;
} Port;

extern Port Ports[65536];

typedef enum WELL_KNOWN_PORTS
{
    PORT_ECHO = 7,
    PORT_DHCP = 68,
    PORT_DNS = 53,
    PORT_HTTP = 80,
    PORT_HTTPS = 443,
    PORT_TELNET = 23,
    PORT_SSH = 22,
    PORT_FTP = 21,
    PORT_SMTP = 25,
    PORT_POP3 = 110,
    PORT_IMAP = 143,
    PORT_NTP = 123,
} PORTS;

/**
 * @brief IP to string
 *
 * @param ip_str The buffer to store the output (at least 20 bytes)
 * @param ip The IP address
 * @return int The number of characters written
 */
int get_ip_str(char *ip_str, uint8_t *ip);

#endif
