#include "audio.h"

/**
 * @see https://wiki.osdev.org/PC_Speaker
 */
void beep(uint32_t freq)
{
    if (!freq)
    {
        output_byte(0x61, input_byte(0x61) & 0xFC);
        return;
    }

    // Set the PIT to the desired frequency
    uint32_t div = 1193180 / freq;
    output_byte(0x43, 0xB6);
    output_byte(0x42, (uint8_t)(div));
    output_byte(0x42, (uint8_t)(div >> 8));

    // And play the sound using the PC speaker
    uint8_t tmp = input_byte(0x61);
    if (tmp != (tmp | 3))
    {
        output_byte(0x61, tmp | 3);
    }
}
