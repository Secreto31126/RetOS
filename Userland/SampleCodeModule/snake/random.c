#include "random.h"
static unsigned int seed = 0x50;
static unsigned int bit;
void setSeed(unsigned int newSeed)
{
    seed = newSeed;
}
unsigned int rand()
{
    bit = ((seed >> 0) ^ (seed >> 2) ^ (seed >> 3) ^ (seed >> 5)) & 1;
    return seed = (seed >> 1) | (bit << 15);
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
    unsigned int got = rand(), r, g, b;
    r = ((got >> 8) & 0xF) * 0x10;
    g = ((got >> 4) & 0xF) * 0x10;
    b = ((got) & 0xF) * 0x10;
    return 0xFF000000 | ((r << 8) & 0x00FF0000) | ((g << 4) & 0x0000FF00) | ((b) & 0x000000FF);
}