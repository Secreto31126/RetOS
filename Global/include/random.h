#ifndef RND_H
#define RND_H
#include <stdint.h>
void setSeed(unsigned int newSeed);
unsigned int rand();
unsigned int randBetween(unsigned int min, unsigned int max);
uint64_t getHexColor();

#endif
