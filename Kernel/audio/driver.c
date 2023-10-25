#include "audio.h"

/**
 * @see https://wiki.osdev.org/PC_Speaker
 */
static void play_sound(uint32_t freq)
{
    // Set the PIT to the desired frequency
    uint32_t div = 1193182 / freq;
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

// make it shutup
static void nosound()
{
    uint8_t tmp = input_byte(0x61) & 0xFC;
    output_byte(0x61, tmp);
}

void beep(uint32_t frequency, uint64_t duration)
{
    play_sound(frequency);
    sleep_ticks(duration);
    nosound();
    // set_PIT_2(old_frequency);
}
