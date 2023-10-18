#include "video.h"

HexColor redRectangleFigure(int x, int y)
{
    return 0xFFFF0000;
}

HexColor greenRectangleFigure(int x, int y)
{
    return 0xFF00FF00;
}

HexColor blueRectangleFigure(int x, int y)
{
    return 0xFF0000FF;
}

HexColor redLine(int x, int y)
{
    if (x < 10 && y < 1)
        return 0xFF000000 | HEX_RED;
    return 0;
}

HexColor redCircle(int x, int y)
{
    x -= 100;
    y -= 100;
    if ((x * x + y * y) < 10000)
        return 0xFF000000 | HEX_RED;
    return 0;
}

HexColor drawEllipse(int x, int y)
{
    x -= 100;
    y -= 25;
    if ((x * x + 16 * (y * y)) < 10000)
        return 0xFF000000 | ((0xFFFFFF * x * y) & 0x00FFFFFF);
    return 0;
}

HexColor RetOSbackground(int x, int y)
{
    if (!(x * y))
        return 0;
    return 0xFF000000 | ((((0xFFFFFF * x * y) & 0x00FFFFFF)) ^ 0x00FFFFFF);
}
