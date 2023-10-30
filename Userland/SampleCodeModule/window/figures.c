#include "window.h"
#include "figures.h"
#include "backgroundArrays.h"

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
HexColor RetOSBackground(int x, int y, double xScaleFactor, double yScaleFactor)
{
    if (!((int)(x * xScaleFactor * y * yScaleFactor)))
        return 0;
    return 0xFF000000 | ((((int)(0xFFFFFF * x * y * xScaleFactor * yScaleFactor) & 0x00FFFFFF)) ^ 0x00FFFFFF);
}

HexColor colorMapper(HexColor *colorMap, int code)
{
    return colorMap[code];
}
static uint64_t w, h;
static double windowsXRatio, windowsYRatio;
updateFigures()
{
    w = getScreenWidth();
    h = getScreenHeight();
    windowsXRatio = BACKGROUND_WIDTH / (double)w;
    windowsYRatio = BACKGROUND_HEIGHT / (double)h;
}
HexColor windowsBackground(int x, int y, double xScaleFactor, double yScaleFactor)
{
    return colorMapper(windowsColorMap, windowsArray[(int)(y * windowsYRatio)][(int)(x * windowsXRatio)]);
}
