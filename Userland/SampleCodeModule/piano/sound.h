#ifndef SND_H
#define SND_H
#include <stdint.h>
void play(uint32_t freq);
void shut();
void playFor(uint32_t freq, uint32_t ticks);
void switchSounds(uint32_t *freq, uint16_t cant, uint32_t ticks);

#endif