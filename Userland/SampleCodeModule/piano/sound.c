#include "sound.h"

#define SHH 0
#define C3 262
#define C3sus 278
#define D3 294
#define D3sus 312
#define E3 330
#define F3 350
#define F3sus 370
#define G3 392
#define G3sus 416
#define A3 440
#define A3sus 466
#define B3 494
#define C4 524
#define C4sus 554
#define D4 588
#define D4sus 622
#define E4 660
#define F4 698
#define F4sus 740
#define G4 784
#define G4sus 830
#define A4 880
#define A4sus 932
#define B4 988
#define C5 1046

// #define MEEP(f, d); beep(f); sleep_ticks(d);

void play(uint32_t freq);
void shut();
void playFor(uint32_t freq, uint32_t ticks);
void playSwitch(uint32_t *freqs, uint32_t cant, uint32_t length);

/**
 * @brief Let's a sound start playing (should be shut down with shut())
 *
 * @param freq frecuency of the sound
 */
void play(uint32_t freq) { beep(freq); }

/**
 * @brief  Stops the sound
 *
 */
void shut() { beep(0); }

/**
 * @brief Plays a sound for a certain amount of time
 *
 * @param freq frecuency of the sound
 * @param ticks amount of ticks the sound will be played
 */
void playFor(uint32_t freq, uint32_t ticks) {
  // uint32_t start = get_tick();
  beep(freq);
  usleep(ticks);
  shut();
}

/**
 * @brief Plays sounds of different frecuencies for 2 ticks each, in a loop for
 * a certain amount of time
 *
 * @param freqs frecuencies of the sounds
 * @param cant amount of frecuencies
 * @param length amount of ticks the sound will be played
 */
void playSwitch(uint32_t *freqs, uint32_t cant, uint32_t length) {
  uint32_t start = get_tick();
  uint32_t i = 0;
  while (get_tick() - start < length) {
    playFor(freqs[i++ % cant], 2);
  }
  shut();
}
