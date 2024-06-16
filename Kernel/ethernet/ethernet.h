#ifndef PETH_H
#define PETH_H

#include <ethernet.h>
#include <lib.h>
#include <console.h>

#include <stdbool.h>

#define IO_ADDRESS 0xC000 // Get from pci search

#define CONFIG_1 0x52 // Configuration Register 1 Address
#define ISR 0x3E      // Interrupt Status Register Start Address
#define CR 0x37       // Command Register Address
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
#define RE_BUFFER_SIZE 8 * (1024) + 16 + 1500 // Osdev suggested size for the recive buffer

#define MESSAGE_BUFFER_SIZE 100 // Internal circular buffer size
#define PAYLOAD_SIZE 10000      // Max payload size

#define BROADCAST_MAC_PARTITION 0xFF // Broadcast MAC value

typedef struct EthernetFrame
{
    uint8_t dest[MAC_SIZE];
    uint8_t src[MAC_SIZE];
    uint16_t length;
    uint8_t payload[PAYLOAD_SIZE]; // CAMBIAR
    uint32_t check;
} EthernetFrame;

/**
 * @brief Get the MAC address object
 *
 * @param macDest The MAC address will be stored here
 */
void get_MAC_address(uint8_t macDest[MAC_SIZE]);
/**
 * @brief Get the MAC address (backwards :)
 *
 * @return uint64_t The MAC address
 */
uint64_t get_MAC_address_64();
/**
 * @brief Check packet ownership
 *
 * @param frame The frame to check
 * @param include_broadcast If true, the broadcast MAC will be tested as well
 * @return true The packet belongs to the device
 * @return false The packet does not belong to the device
 */
bool isPacketDestinedForThisDevice(EthernetFrame *frame, bool include_broadcast);

#endif
