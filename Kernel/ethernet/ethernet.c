#include "ethernet.h"

char tr_buffer[3][TR_BUFFER_SIZE] = {{0}};
char rx_buffer[RE_BUFFER_SIZE] = {0};

uint64_t init_ethernet()
{
    // Enable
    output_byte(IO_ADDRESS + CONFIG_1, 0);

    // Reset
    output_byte(IO_ADDRESS + CR, 0x10);
    while (input_byte(IO_ADDRESS + CR) & 0x10)
        ;

    // Writing locations
    output_dword(IO_ADDRESS + RBSTART, (uint64_t)rx_buffer);

    output_dword(IO_ADDRESS + TSAD0, (uint64_t)tr_buffer[0]);
    output_dword(IO_ADDRESS + TSAD1, (uint64_t)tr_buffer[1]);
    output_dword(IO_ADDRESS + TSAD2, (uint64_t)tr_buffer[2]);
    output_dword(IO_ADDRESS + TSAD3, (uint64_t)tr_buffer[3]);

    // TOK = 0x4, ROK = 0x1
    output_word(IO_ADDRESS + IMR, 0x5);

    // Telling the RTL8139 to accept AB+AM+APM+AAP packets (0xF)
    // Setting WRAP bit, so the packet will be written contiguously in memory
    output_dword(IO_ADDRESS + RCR, 0xf | (1 << 7));

    // Enable recive and transmit
    output_byte(IO_ADDRESS + CR, 0xC);

    return get_MAC_address_64();
}

void get_MAC_address(uint8_t dest[MAC_SIZE])
{
    uint32_t mac = input_dword(IO_ADDRESS);
    dest[0] = mac;
    dest[1] = mac >> 8;
    dest[2] = mac >> 16;
    dest[3] = mac >> 24;
    mac = input_dword(IO_ADDRESS + 4);
    dest[4] = mac;
    dest[5] = mac >> 8;
}

uint64_t get_MAC_address_64()
{
    return ((uint64_t)input_dword(IO_ADDRESS + 4) << 32) + input_dword(IO_ADDRESS);
}

bool isPacketDestinedForThisDevice(EthernetFrame *frame, bool include_broadcast)
{
    uint8_t local_MAC[MAC_SIZE];
    get_MAC_address(local_MAC);

    int flag = include_broadcast ? 3 : 1; // 0b11, high for broadcast, low for own MAC
    for (int i = 0; flag && i < MAC_SIZE; i++)
    {
        if ((flag & 1) && frame->dest[i] != local_MAC[i])
        {
            flag &= 2;
        }

        if ((flag & 2) && frame->dest[i] != BROADCAST_MAC_PARTITION)
        {
            flag &= 1;
        }
    }

    return !!flag;
}
