#ifndef SND_H
#define SND_H

#include <stdint.h>
#include "./../nstdlib/nstdlib.h"

void play(uint32_t freq);
void shut();
void playFor(uint32_t freq, uint32_t ticks);


#endif