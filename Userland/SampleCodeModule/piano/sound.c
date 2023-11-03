#include "sound.h"

/**
 * @brief Let's a sound start playing (should be shut down with shut())
 *
 * @param freq frecuency of the sound
 */
void play(uint32_t freq)
{
    beep(freq);
}

/**
 * @brief  Stops the sound
 *
 */
void shut()
{
    beep(0);
}

/**
 * @brief Plays a sound for a certain amount of time
 *
 * @param freq frecuency of the sound
 * @param ticks amount of ticks the sound will be played
 */
void playFor(uint32_t freq, uint32_t ticks)
{
    uint32_t start = get_tick();
    beep(freq);
    while (get_tick() - start < ticks)
    {
        // do nothing
    }
    shut();
}

/**
 * @brief Plays sounds of different frecuencies for 2 ticks each, in a loop for a certain amount of time
 *
 * @param freqs frecuencies of the sounds
 * @param cant amount of frecuencies
 * @param length amount of ticks the sound will be played
 */
void playSwitch(uint32_t *freqs, uint32_t cant, uint32_t length)
{
    uint32_t start = get_tick();
    while (get_tick() - start < length)
    {
        playFor(freqs[(get_tick() - start) % cant], 2);
    }
    shut();
}