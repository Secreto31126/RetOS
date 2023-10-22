#include "window.h"

#define GET_HEX(a, r, g, b) (((a) << 24) + ((r) << 16) + ((g) << 8) + (b))
#define GET_OPACITY(x) (((x) >> 24) & 0xFF)
#define GET_RED(x) (((x) >> 16) & 0xFF)
#define GET_GREEN(x) (((x) >> 8) & 0xFF)
#define GET_BLUE(x) ((x) & 0xFF)

int putPixelStd(Window w, HexColor color, uint64_t x, uint64_t y) // yes, a lot of this is essentially a paste from the kernel's driver. Proper isolation is still kept. Both systems of representing pixels are essentially the same (since we created both). The only communication between these systems is through syscalls, even if they look the same. We choose to create and manage a parallel window in userland to prevent repeated syscalls and because modelling a screen from a character array,bitmap,etc. in kernel was literally hardcoding graphics into the os. long comment goes brrr
{
    if (x >= w.width || y >= w.height)
        return 0;

    // Gotta go fast(er)
    if (!GET_OPACITY(color))
    {
        return 1;
    }

    w.pixels[y * w.width + x] = mergeColor(w.pixels[y * w.width + x], color);
    return 1;
}

int putPixel(Window w, HexColor color, uint64_t x, uint64_t y)
{
    return putPixelStd(w, color, x, y);
}

uint64_t mergeColor(HexColor background, uint64_t overlay)
{
    uint8_t op = GET_OPACITY(overlay);
    return GET_HEX(0xFF,
                   mergeOneColor(GET_RED(background), GET_RED(overlay), op),
                   mergeOneColor(GET_GREEN(background), GET_GREEN(overlay), op),
                   mergeOneColor(GET_BLUE(background), GET_BLUE(overlay), op));
}
uint8_t mergeOneColor(uint8_t background, uint8_t overlay, uint8_t opacity)
{
    if (!opacity)
        return background;
    double op = (double)opacity / 0xFF;
    return (background * (1 - op) + overlay * op) / 2;
}

void drawShape(Window w, ShapeFunction f, int x, int y, int xRange, int yRange)
{
    drawScaledShape(w, f, x, y, xRange, yRange, 1, 1);
}

void drawScaledShape(Window w, ShapeFunction f, int x, int y, int xRange, int yRange, double xScaleFactor, double yScaleFactor)
{
    if (!xScaleFactor || !yScaleFactor)
        return;

    for (int i = 0; i < yRange && i < w.height; i++)
    {
        for (int j = 0; j < xRange && j < w.width; j++)
        {
            uint32_t r = f(j / xScaleFactor, i / yScaleFactor, xRange, yRange);
            if (r & 0xFF000000)
                putPixel(w, r, x + j, y + i);
        }
    }
}