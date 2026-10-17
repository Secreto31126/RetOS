#include "ethernet.h"

#include "physical/rtl8139.h"
#include "application/echo.h"
#include "application/dhcp.h"
#include "application/dsh.h"

#include <semaphores.h>
#include <stdio.h>

#define RX_QUEUE_SLOTS 32
#define RX_SLOT_SIZE 1536

typedef struct RXSlot
{
    uint16_t length;
    uint8_t data[RX_SLOT_SIZE];
} RXSlot;

static RXSlot rx_queue[RX_QUEUE_SLOTS];
static uint16_t rx_head = 0;
static uint16_t rx_tail = 0;
static uint16_t rx_count = 0;

static sem_t rx_sem;

static void add_port_handler(uint16_t port, uint16_t protocol, PORT_HANDLER handler)
{
    Ports[port].protocol = protocol;
    Ports[port].handler = handler;
}

// Interrupts must be disabled
static void process_frame()
{
    RXSlot *slot = &rx_queue[rx_head];

    // Released after processing so the ISR can't overwrite it
    receive((EthernetFrame *)slot->data, slot->length);

    rx_head = (rx_head + 1) % RX_QUEUE_SLOTS;
    rx_count--;
}

Port Ports[MAX_PORTS] = {{0}};

uint64_t init_ethernet()
{
    sem_init(&rx_sem, 1, 0);

    init_rtl8139();
    init_arp();

    add_port_handler(PORT_ECHO, PROTOCOL_UDP, (PORT_HANDLER)echo_handle_packet);
    add_port_handler(PORT_DHCP, PROTOCOL_UDP, (PORT_HANDLER)dhcp_handle_packet);
    add_port_handler(3000, PROTOCOL_UDP, (PORT_HANDLER)dsh_handle_packet);

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

void ethernet_enqueue(const void *frame, uint16_t length)
{
    if (length > RX_SLOT_SIZE)
    {
        return;
    }

    uint64_t flags = interrupts_disable();

    if (rx_count < RX_QUEUE_SLOTS)
    {
        RXSlot *slot = &rx_queue[rx_tail];
        memcpy(slot->data, frame, length);
        slot->length = length;

        rx_tail = (rx_tail + 1) % RX_QUEUE_SLOTS;
        rx_count++;

        sem_post(&rx_sem);
    }

    interrupts_restore(flags);
}

void ethernet_daemon()
{
    while (1)
    {
        // rx_sem is posted from the ISR, never hold it with interrupts on
        uint64_t flags = interrupts_disable();

        if (!sem_wait(&rx_sem) && rx_count)
        {
            process_frame();
        }

        interrupts_restore(flags);
    }
}

void ethernet_poll()
{
    uint64_t flags = interrupts_disable();

    int pending = 0;
    while (!sem_getvalue(&rx_sem, &pending) && pending > 0 && !sem_wait(&rx_sem))
    {
        if (rx_count)
        {
            process_frame();
        }
    }

    interrupts_restore(flags);
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
