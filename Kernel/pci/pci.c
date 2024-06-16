#include <pci.h>
#include <lib.h>
#include <console.h>
#include <stdbool.h>

#define RTL_VENDOR_ID 0x10EC
#define RTL_DEVICE_ID 0x8139

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

/**
 * @brief Active DMA for any bus and slot
 */
void active_dma(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;

    // Create configuration address
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    // Write out the address
    output_dword(PCI_CONFIG_ADDRESS, address);

    // Write the command register
    output_dword(PCI_CONFIG_DATA, 0x107);
}

void init_dma()
{
    // Active DMA for RTL8139 - BUS=0, SLOT=3, FUNCTION=0, CMDreg=4
    active_dma(0, 3, 0, 4);
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
