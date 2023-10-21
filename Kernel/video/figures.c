#include "video.h"

HexColor redRectangleFigure()
{
    return 0xFFFF0000;
}

HexColor greenRectangleFigure()
{
    return 0xFF00FF00;
}

HexColor blueRectangleFigure()
{
    return 0xFF0000FF;
}

HexColor redLine(int x, int y)
{
    if (y < 1)
        return 0xFF000000 | HEX_RED;
    return 0;
}

HexColor drawCircle(int x, int y, int w, int h)
{
    w /= 2;
    h /= 2;
    x -= w;
    y -= h;
    if (((double)(x * x) / (w * w)) + ((double)(y * y) / (h * h)) < 1)
        return 0xFF000000;
    return 0;
}

HexColor redCircle(int x, int y, int w, int h)
{
    return drawCircle(x, y, w, h) | HEX_RED;
}

HexColor drawEllipse(int x, int y)
{
    x -= 100;
    y -= 25;
    if ((x * x + 16 * (y * y)) < 10000)
        return 0x50000000 | ((0xFFFFFF * x * y) & 0x00FFFFFF);
    return 0;
}

HexColor RetOSbackground(int x, int y)
{
    if (!(x * y))
        return 0;
    return 0xFF000000 | ((((0xFFFFFF * x * y) & 0x00FFFFFF)) ^ 0x00FFFFFF);
}
