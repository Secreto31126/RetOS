#include "rtl8139.h"

#include <pci.h>

#define CONFIG_1 0x52 // Configuration Register 1 Address
#define ISR 0x3E      // Interrupt Status Register Start Address
#define CR 0x37       // Command Register Address
#define CAPR 0x38     // Idk
#define IMR 0x3C      // Interrupt Mask Register
#define RCR 0x44      // Receive (Rx) Configuration Register

#define RBSTART 0x30 // Receive Buffer Start Address
#define TSAD0 0x20   // Transmit Start Address of Descriptor0
#define TSAD1 0x24   // Transmit Start Address of Descriptor1
#define TSAD2 0x28   // Transmit Start Address of Descriptor2
#define TSAD3 0x2C   // Transmit Start Address of Descriptor3

#define TSD0 0x10 // Transmit Status of Descriptor 0
#define TSD1 0x14 // Transmit Status of Descriptor 1
#define TSD2 0x18 // Transmit Status of Descriptor 2
#define TSD3 0x1C // Transmit Status of Descriptor 3

#define CHECK_TOK 0x4    // Packet transmision completed succesfully
#define CHECK_ROK 0x1    // Succesfull completition of packet reception
#define CHECK_RXOVW 0x10 // Receive buffer overflow
#define CHECK_FOVW 0x40  // Receive FIFO overflow

#define CR_BUFE 0x1       // Receive buffer empty
#define TSD_OWN 0x2000    // DMA of the descriptor's data completed
#define RX_STATUS_ROK 0x1 // Received packet is good

#define RTL8139_IRQ 11

#define TR_BUFFER_SIZE 8 * (1024)             // Osdev suggested size for the transmit buffer
#define RX_BUFFER_SIZE 8 * (1024) + 16 + 1500 // Osdev suggested size for the recive buffer
#define RX_RING_SIZE (8 * 1024)

#define MIN_FRAME_SIZE 60      // Without CRC
#define MAX_TX_FRAME_SIZE 1792 // Larger frames stall the TX queue
#define MIN_RX_FRAME_SIZE 64   // With CRC
#define MAX_RX_FRAME_SIZE 1522 // With CRC and 802.1Q tag
#define CRC_SIZE 4

#define MESSAGE_BUFFER_SIZE 100 // Internal circular buffer size
#define PAYLOAD_SIZE 10000      // Max payload size

static uint8_t mac_address[MAC_LENGTH];

static char tr_buffer[MAX_CONCURRENT_PACKETS][TR_BUFFER_SIZE] = {{0}};
static char rx_buffer[RX_BUFFER_SIZE] = {0};

static uint8_t TSD_registers[4] = {TSD0, TSD1, TSD2, TSD3};
static uint8_t TSAD_registers[4] = {TSAD0, TSAD1, TSAD2, TSAD3};

static uint16_t io_base = 0;

static uint32_t current_rx_ptr = 0;
static uint32_t current_tx = 0;
static bool tx_used[MAX_CONCURRENT_PACKETS] = {0};

void init_rtl8139()
{
    PCIDevice device;
    if (!pci_find_device(RTL_VENDOR_ID, RTL_DEVICE_ID, &device))
    {
        ncPrint("\tError: RTL8139 not found, ethernet disabled\n");
        return;
    }

    if (device.interrupt_line != RTL8139_IRQ)
    {
        ncPrint("\tWarning: RTL8139 uses IRQ ");
        ncPrintDec(device.interrupt_line);
        ncPrint(", only IRQ 11 is handled\n");
    }

    pci_enable_device(&device);
    io_base = device.io_base;

    // Enable
    output_byte(io_base + CONFIG_1, 0);

    // Reset
    output_byte(io_base + CR, 0x10);
    while (input_byte(io_base + CR) & 0x10)
        ;

    // Writing locations
    output_dword(io_base + RBSTART, (uint64_t)rx_buffer);

    output_word(io_base + IMR, CHECK_TOK | CHECK_ROK | CHECK_RXOVW | CHECK_FOVW);

    // Telling the RTL8139 to accept AB+AM+APM+AAP packets (0xF)
    // Setting WRAP bit, so the packet will be written contiguously in memory
    output_dword(io_base + RCR, 0xF | (1 << 7));

    // Enable recive and transmit
    output_byte(io_base + CR, 0xC);

    // Save the MAC address
    uint32_t mac = input_dword(io_base);
    mac_address[0] = mac;
    mac_address[1] = mac >> 8;
    mac_address[2] = mac >> 16;
    mac_address[3] = mac >> 24;
    mac = input_dword(io_base + 4);
    mac_address[4] = mac;
    mac_address[5] = mac >> 8;
}

void rtl8139_get_MAC_address(uint8_t mac[MAC_LENGTH])
{
    memcpy(mac, mac_address, MAC_LENGTH);
}

int rtl8139_send_packet(void *data, uint32_t length)
{
    if (!io_base || !length || length > MAX_TX_FRAME_SIZE)
    {
        return -1;
    }

    uint64_t flags = interrupts_disable();

    if (tx_used[current_tx] && !(input_dword(io_base + TSD_registers[current_tx]) & TSD_OWN))
    {
        interrupts_restore(flags);
        return -1;
    }

    // First, copy the data to a physically contiguous chunk of memory
    char *packet = tr_buffer[current_tx];
    memcpy(packet, data, length);

    uint32_t frame_length = length;
    if (frame_length < MIN_FRAME_SIZE)
    {
        // The chip doesn't pad short frames
        memset(packet + frame_length, 0, MIN_FRAME_SIZE - frame_length);
        frame_length = MIN_FRAME_SIZE;
    }

    // Second, fill in physical address of data, and length
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
    output_dword(io_base + TSAD_registers[current_tx], (uint32_t)packet);
#pragma GCC diagnostic pop

    output_dword(io_base + TSD_registers[current_tx], frame_length);

    tx_used[current_tx] = true;
    current_tx = (current_tx + 1) % MAX_CONCURRENT_PACKETS;

    interrupts_restore(flags);

    return length;
}

void rtl8139_receive_packet()
{
    while (!(input_byte(io_base + CR) & CR_BUFE))
    {
        uint8_t *received = (uint8_t *)rx_buffer + current_rx_ptr;
        uint16_t status = received[0] | (received[1] << 8);
        uint16_t length = received[2] | (received[3] << 8);

        if (!(status & RX_STATUS_ROK) || length < MIN_RX_FRAME_SIZE || length > MAX_RX_FRAME_SIZE)
        {
            return;
        }

        ethernet_enqueue(received + 4, length - CRC_SIZE);

        current_rx_ptr = ((current_rx_ptr + length + 4 + 3) & ~3) % RX_RING_SIZE;

        output_word(io_base + CAPR, (uint16_t)(current_rx_ptr - 0x10));
    }
}

void rtl8139_handle_interrupt()
{
    if (!io_base)
    {
        return;
    }

    uint16_t status = input_word(io_base + ISR);
    output_word(io_base + ISR, status);

    if (status & (CHECK_ROK | CHECK_RXOVW | CHECK_FOVW))
    {
        rtl8139_receive_packet();
    }
}
