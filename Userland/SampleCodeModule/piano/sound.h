#ifndef SND_H
#define SND_H
#include <stdint.h>
void play(uint32_t freq);
void shut();
void playFor(uint32_t freq, uint32_t ticks);
void switchSounds(uint32_t *freq, uint16_t cant, uint32_t ticks);
void imperial_march();
void hes_a_pirate();
void outer_wilds();
void do_i_wanna_know();
void whistle_stop();
void here_comes_the_sun();

#endif