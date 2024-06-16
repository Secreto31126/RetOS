#ifndef ETH_H
#define ETH_H

#include <stdint.h>

#define MAC_SIZE 6

typedef struct
{
    uint8_t mac[MAC_SIZE];
    uint8_t message[1000];
    uint16_t length;
    uint8_t type;
} EthernetMessage;

/**
 * @brief Initializes the ethernet module
 *
 * @return uint64_t The MAC address
 */
uint64_t init_ethernet();
// void send_ethernet_message(EthernetMessage msg);
// int get_ethernet_message(EthernetMessage *msg);

void ethernet_manager();

#endif
