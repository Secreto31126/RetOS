#include "random.h"
static unsigned int seed = 0x50;
static unsigned int bit;
void setSeed(unsigned int newSeed)
{
    seed = newSeed;
}
unsigned int rand()
{ // A linear-feedback shift register, sourced from https://en.wikipedia.org/wiki/Linear-feedback_shift_register
    bit = ((seed >> 0) ^ (seed >> 2) ^ (seed >> 3) ^ (seed >> 5)) & 1;
    return (seed = (seed >> 1) | (bit << 15));
}
unsigned int randBetween(unsigned int min, unsigned int max)
{
    if (min >= max)
        return 0;
    unsigned int got = rand();
    return (got % (max - min)) + min;
}
uint64_t getHexColor()
{
    return 0xFF000000 | ((rand() << 16) & 0x00FF0000) | ((rand() << 8) & 0x0000FF00) | (rand() & 0x000000FF);
}