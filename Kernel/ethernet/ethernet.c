#include "ethernet.h"

#include "physical/rtl8139.h"
#include "application/echo.h"
#include "application/dhcp.h"

#include <stdio.h>

static void add_port_handler(uint16_t port, uint16_t protocol, PORT_HANDLER handler)
{
    Ports[port].protocol = protocol;
    Ports[port].handler = handler;
}

Port Ports[MAX_PORTS] = {{0}};

uint64_t init_ethernet()
{
    init_rtl8139();
    init_arp();

    add_port_handler(PORT_ECHO, PROTOCOL_UDP, (PORT_HANDLER)echo_handle_packet);
    add_port_handler(PORT_DHCP, PROTOCOL_UDP, (PORT_HANDLER)dhcp_handle_packet);

    return get_MAC_address_64();
}

void get_MAC_address(uint8_t mac[MAC_LENGTH])
{
    rtl8139_get_MAC_address(mac);
}

uint64_t get_MAC_address_64()
{
    uint8_t mac[MAC_LENGTH];
    get_MAC_address(mac);

    uint64_t result = 0;
    for (int i = 0; i < MAC_LENGTH; i++)
    {
        result <<= 8;
        result |= mac[i];
    }

    return result;
}

void ethernet_manager()
{
    rtl8139_handle_interrupt();
}

void memdump(void *ptr, int length)
{
    uint8_t *data = (uint8_t *)ptr;
    for (int i = 0; i < length; i++)
    {
        if (i && !(i % 8))
        {
            ncNewline();
        }

        char buf[4];
        snprintf(buf, 4, "%02x ", data[i]);
        ncPrint(buf);
    }

    ncNewline();
}
