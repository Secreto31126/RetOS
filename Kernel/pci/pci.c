#include <console.h>
#include <lib.h>
#include <pci.h>
#include <stdbool.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

#define PCI_COMMAND 0x04
#define PCI_BAR0 0x10
#define PCI_INTERRUPT_LINE 0x3C

#define PCI_COMMAND_IO_SPACE 0x1
#define PCI_COMMAND_BUS_MASTER 0x4

static uint32_t pci_config_address(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    return (uint32_t)(((uint32_t)bus << 16) | ((uint32_t)slot << 11) | ((uint32_t)func << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
}

static uint32_t pci_config_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    output_dword(PCI_CONFIG_ADDRESS, pci_config_address(bus, slot, func, offset));
    return input_dword(PCI_CONFIG_DATA);
}

static void pci_config_write_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t value)
{
    output_dword(PCI_CONFIG_ADDRESS, pci_config_address(bus, slot, func, offset));
    output_word(PCI_CONFIG_DATA + (offset & 2), value);
}

bool pci_find_device(uint16_t vendor_id, uint16_t device_id, PCIDevice *device)
{
    for (int bus = 0; bus < 256; bus++)
    {
        for (int slot = 0; slot < 32; slot++)
        {
            uint32_t id = pci_config_read_dword(bus, slot, 0, 0);
            if ((id & 0xFFFF) != vendor_id || (id >> 16) != device_id)
            {
                continue;
            }

            device->bus = bus;
            device->slot = slot;
            device->io_base = pci_config_read_dword(bus, slot, 0, PCI_BAR0) & ~0x3;
            device->interrupt_line = pci_config_read_dword(bus, slot, 0, PCI_INTERRUPT_LINE) & 0xFF;
            return true;
        }
    }

    return false;
}

void pci_enable_device(const PCIDevice *device)
{
    uint16_t command = pci_config_read_dword(device->bus, device->slot, 0, PCI_COMMAND) & 0xFFFF;
    pci_config_write_word(device->bus, device->slot, 0, PCI_COMMAND, command | PCI_COMMAND_IO_SPACE | PCI_COMMAND_BUS_MASTER);
}

// Searching RTL8139 functions

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;

    // Create configuration address
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    // Write out the address
    output_dword(PCI_CONFIG_ADDRESS, address);

    // Read in the data
    return (input_dword(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF;
}

bool pciCheckVendor(uint8_t bus, uint8_t slot, uint16_t vendor_id, uint16_t device_id)
{
    return pciConfigReadWord(bus, slot, 0, 0) == vendor_id && pciConfigReadWord(bus, slot, 0, 2) == device_id;
}

void print_RTL_data()
{
    if (!pciCheckVendor(0, 3, RTL_VENDOR_ID, RTL_DEVICE_ID))
        return ncPrint("RTL8139 not found at 0:3\n");

    ncPrint("\tHeader: ");
    ncPrintDec(pciConfigReadWord(0, 3, 0, 0xC));

    for (uint16_t i = 0; i < 6; i++)
    {
        ncPrint("\n\tBAR");
        ncPrintDec(i);
        ncPrint(": ");
        // ncPrintHex(pciConfigReadWord(0, 3, 0, 0x12 + 4 * i));
        // ncTab();
        // ncPrintHex(pciConfigReadWord(0, 3, 0, 0x10 + 4 * i));
        ncPrintHex(((((uint32_t)pciConfigReadWord(0, 3, 0, 0x12 + 4 * i)) << 16) | pciConfigReadWord(0, 3, 0, 0x10 + 4 * i)) & 0xFFFFFFFC);
    }

    ncNewline();
}

void scan_PCI_devices()
{
    for (int bus = 0; bus < 256; bus++)
    {
        for (int slot = 0; slot < 32; slot++)
        {
            uint16_t vendor_id = pciConfigReadWord(bus, slot, 0, 0);
            if (vendor_id != 0xFFFF)
            {
                ncPrint("\tVendorID: 0x");
                ncPrintHex(vendor_id);
                ncPrint("\tDeviceID: 0x");
                ncPrintHex(pciConfigReadWord(bus, slot, 0, 2));
                ncPrint("\tBus: ");
                ncPrintDec(bus);
                ncPrint("\tSlot: ");
                ncPrintDec(slot);
                ncPrint("\tHeader: ");
                ncPrintDec(pciConfigReadWord(bus, slot, 0, 0xC));
                ncNewline();
            }
        }
    }
}
