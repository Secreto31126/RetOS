#ifndef PCI_H
#define PCI_H

#include <stdbool.h>
#include <stdint.h>

#define RTL_VENDOR_ID 0x10EC
#define RTL_DEVICE_ID 0x8139

typedef struct PCIDevice
{
    uint8_t bus;
    uint8_t slot;
    uint16_t io_base;
    uint8_t interrupt_line;
} PCIDevice;

/**
 * @brief Search buses 0-255, slots 0-31 (function 0) for a device
 *
 * @param vendor_id The vendor ID
 * @param device_id The device ID
 * @param device Where to store the device's location, BAR0 I/O base and interrupt line
 * @return true The device was found
 * @return false The device was not found
 */
bool pci_find_device(uint16_t vendor_id, uint16_t device_id, PCIDevice *device);
/**
 * @brief Enable I/O space access and bus mastering (DMA) for a device
 *
 * @param device The device
 */
void pci_enable_device(const PCIDevice *device);
void scan_PCI_devices();
void print_RTL_data();

#endif
