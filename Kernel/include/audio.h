#ifndef AIO_H
#define AIO_H

/**
 * @brief Plays a beep sound asynchronously
 *
 * @param frequency The frequency of the beep
 * @param duration The duration of the beep in ticks
 */
void beep(uint32_t frequency, uint64_t duration);

/**
 * @brief Plays a beep sound synchronously
 *
 * @param frequency The frequency of the beep
 * @param duration The duration of the beep in ticks
 */
void beep_syncronic(uint32_t frequency, uint64_t duration);

void imperial_march();

#endif
