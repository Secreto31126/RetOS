#include "rtl8139.h"

#define IO_ADDRESS 0xC000 // Get from pci search

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

#define CHECK_TOK 0x4 // Packet transmision completed succesfully
#define CHECK_ROK 0x1 // Succesfull completition of packet reception

#define CLEAR_TOK 0xFB  // Clearing the Transmit OK bit
#define CLEAR_ROK 0xFE  // Clearing the Recive OK bit
#define CLEAR_OWN 0xFFF // Clearing the OWN bit

#define TR_BUFFER_SIZE 8 * (1024)             // Osdev suggested size for the transmit buffer
#define RX_BUFFER_SIZE 8 * (1024) + 16 + 1500 // Osdev suggested size for the recive buffer

#define MESSAGE_BUFFER_SIZE 100 // Internal circular buffer size
#define PAYLOAD_SIZE 10000      // Max payload size

static uint8_t mac_address[MAC_LENGTH];

static char tr_buffer[MAX_CONCURRENT_PACKETS][TR_BUFFER_SIZE] = {{0}};
static char rx_buffer[RX_BUFFER_SIZE] = {0};

static uint8_t TSD_registers[4] = {TSD0, TSD1, TSD2, TSD3};
static uint8_t TSAD_registers[4] = {TSAD0, TSAD1, TSAD2, TSAD3};

static uint32_t current_rx_ptr = 0;
static uint32_t current_tx = 0;

static int sending = 0;

void init_rtl8139()
{
    // Enable
    output_byte(IO_ADDRESS + CONFIG_1, 0);

    // Reset
    output_byte(IO_ADDRESS + CR, 0x10);
    while (input_byte(IO_ADDRESS + CR) & 0x10)
        ;

    // Writing locations
    output_dword(IO_ADDRESS + RBSTART, (uint64_t)rx_buffer);

    // output_dword(IO_ADDRESS + TSAD0, (uint64_t)tr_buffer[0]);
    // output_dword(IO_ADDRESS + TSAD1, (uint64_t)tr_buffer[1]);
    // output_dword(IO_ADDRESS + TSAD2, (uint64_t)tr_buffer[2]);
    // output_dword(IO_ADDRESS + TSAD3, (uint64_t)tr_buffer[3]);

    // TOK = 0x4, ROK = 0x1
    output_word(IO_ADDRESS + IMR, CHECK_TOK | CHECK_ROK);

    // Telling the RTL8139 to accept AB+AM+APM+AAP packets (0xF)
    // Setting WRAP bit, so the packet will be written contiguously in memory
    output_dword(IO_ADDRESS + RCR, 0xF | (1 << 7));

    // Enable recive and transmit
    output_byte(IO_ADDRESS + CR, 0xC);

    // Save the MAC address
    uint32_t mac = input_dword(IO_ADDRESS);
    mac_address[0] = mac;
    mac_address[1] = mac >> 8;
    mac_address[2] = mac >> 16;
    mac_address[3] = mac >> 24;
    mac = input_dword(IO_ADDRESS + 4);
    mac_address[4] = mac;
    mac_address[5] = mac >> 8;
}

void rtl8139_get_MAC_address(uint8_t mac[MAC_LENGTH])
{
    memcpy(mac, mac_address, MAC_LENGTH);
}

int rtl8139_send_packet(void *data, uint32_t length)
{
    if (sending + 1 > MAX_CONCURRENT_PACKETS)
    {
        return -1;
    }

    sending++;

    // First, copy the data to a physically contiguous chunk of memory
    void *packet = tr_buffer[current_tx];
    memcpy(packet, data, length);

    // Second, fill in physical address of data, and length
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
    output_dword(IO_ADDRESS + TSAD_registers[current_tx], (uint32_t)packet);
#pragma GCC diagnostic pop

    output_dword(IO_ADDRESS + TSD_registers[current_tx++], length);

    if (current_tx > 3)
    {
        current_tx = 0;
    }

    return length;
}

void rtl8139_receive_packet()
{
    uint16_t *received = (uint16_t *)(rx_buffer + current_rx_ptr);
    uint16_t length = *(received + 1);

    void *packet = malloc(length);
    if (!packet)
    {
        return;
    }

    memcpy(packet, received + 2, length);

    receive(packet, length);

    free(packet);

    current_rx_ptr = (current_rx_ptr + length + 4 + 3) & (~3);

    if (current_rx_ptr > RX_BUFFER_SIZE)
    {
        current_rx_ptr -= RX_BUFFER_SIZE;
    }

    output_word(IO_ADDRESS + CAPR, current_rx_ptr - 0x10);
}

void rtl8139_handle_interrupt()
{
    uint16_t status = input_word(IO_ADDRESS + ISR);
    output_word(IO_ADDRESS + ISR, 0x5);

    if (status & CHECK_ROK)
    {
        rtl8139_receive_packet();
        // output_word(IO_ADDRESS + ISR, status & CLEAR_ROK);
    }

    if (status & CHECK_TOK)
    {
        sending--;
        // output_word(IO_ADDRESS + ISR, status & CLEAR_TOK);
    }
}
