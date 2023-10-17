#include "video.h"

uint32_t drawRedLine(int x, int y)
{
    if (x < 10 && y < 1)
        return 0xFF000000 | HEX_RED;
    return 0;
}

uint32_t drawRedCircle(int x, int y)
{
    x -= 100;
    y -= 100;
    if ((x * x + y * y) < 10000)
        return 0xFF000000 | HEX_RED;
    return 0;
}

uint32_t drawAnEllipse(int x, int y)
{
    x -= 100;
    y -= 25;
    if ((x * x + 16 * (y * y)) < 10000)
        return 0xFF000000 | ((0xFFFFFF * x * y) & 0x00FFFFFF);
    return 0;
}

uint32_t drawACanvas(int x, int y)
{
    return (0xFF000000 | (((0xFFFFFF * x * y) & 0x00FFFFFF))) ^ 0x00FFFFFF;
}
