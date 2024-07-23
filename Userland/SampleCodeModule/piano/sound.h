#ifndef SND_H
#define SND_H

#include "./../nstdlib/nstdlib.h"
#include <stdint.h>

void play(uint32_t freq);
void shut();
void playFor(uint32_t freq, uint32_t ticks);

#endif