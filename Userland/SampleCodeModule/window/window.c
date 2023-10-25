#include "window.h"
#include <stdarg.h>

#define GET_HEX(a, r, g, b) (((a) << 24) + ((r) << 16) + ((g) << 8) + (b))
#define GET_OPACITY(x) (((x) >> 24) & 0xFF)
#define GET_RED(x) (((x) >> 16) & 0xFF)
#define GET_GREEN(x) (((x) >> 8) & 0xFF)
#define GET_BLUE(x) ((x) & 0xFF)

extern uint64_t get_screen_size();
extern uint64_t draw(HexColor *figure, uint64_t dimensions, uint64_t position);

uint8_t mergeOneColor(uint8_t background, uint8_t overlay, uint8_t opacity)
{
    if (!opacity)
        return background;
    double op = (double)opacity / 0xFF;
    return (background * (1 - op) + overlay * op);
}
uint64_t mergeColor(HexColor background, uint64_t overlay)
{
    uint8_t op = GET_OPACITY(overlay);
    return GET_HEX(0xFF,
                   mergeOneColor(GET_RED(background), GET_RED(overlay), op),
                   mergeOneColor(GET_GREEN(background), GET_GREEN(overlay), op),
                   mergeOneColor(GET_BLUE(background), GET_BLUE(overlay), op));
}

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
void drawShape(Window w, ShapeFunction f, int x, int y, int xRange, int yRange)
{
    drawScaledShape(w, f, x, y, xRange, yRange, 1, 1);
}

void drawFromHexArray(Window w, HexColor *source, int sourceWidth, int sourceHeight, int x, int y, double xScaleFactor, double yScaleFactor)
{
    if (!xScaleFactor || !yScaleFactor)
        return;
    xScaleFactor = 1 / xScaleFactor; // scaleFactor 2 means one pixel in source represents two pixels in window
    yScaleFactor = 1 / yScaleFactor;
    uint64_t xIndex, yIndex = y;
    for (double i = 0; i < sourceHeight && yIndex < w.height; i += yScaleFactor)
    {
        xIndex = x;
        for (double j = 0; j < sourceWidth && xIndex < w.width; j += xScaleFactor)
        {
            putPixel(w, source[((int)(i + 0.5)) * sourceWidth + ((int)(j + 0.5))], xIndex, yIndex); // source pointers rounded to nearest integer
            xIndex++;
        }
        yIndex++;
    }
}
/**
 * Automatically scales source to result
 * To make a color in the result be 'transparent' in later operations, set its HexColor opacity to 0.
 * This function is for translating bitmap-equivalents to HexColor arrays. Not for creating Windows.
 * Chars set to 0 will be copied to result as color 1, set to 1 copied as color 2, etc.
 **/
HexColor *toHexArray(char *source, HexColor *result, int sourceWidth, int sourceHeight, int resultWidth, int resultHeight, int colorCount, ...) // Requiring the number of colors isn't a particularly good implementation, but it is a particularly easy one.
{
    HexColor replacement;
    va_list colors;
    va_start(colors, colorCount);
    double xScaleFactor = (double)resultWidth / sourceWidth, yScaleFactor = (double)resultHeight / sourceHeight;
    for (int c = 0; c < colorCount; c++)
    {
        replacement = va_arg(colors, HexColor);
        int xIndex, yIndex = 0;
        for (double i = 0; i < resultHeight && yIndex < sourceHeight; i += xScaleFactor)
        {
            xIndex = 0;
            for (double j = 0; j < resultWidth && xIndex < sourceWidth; j += yScaleFactor)
            {
                if (source[xIndex + yIndex * sourceWidth] == c)
                    result[(int)(j + 0.5) + (int)(i + 0.5) * resultWidth] = replacement;
                xIndex++;
            }
            yIndex++;
        }
    }
    return result;
}

uint64_t getScreenWidth()
{
    return (get_screen_size() >> 16);
}
uint64_t getScreenHeight()
{
    return (get_screen_size()) & 0xFFFF;
}
uint64_t drawWindow(Window w, uint64_t x, uint64_t y)
{
    return draw(w.pixels, ((w.width << 16) & 0xFFFF0000) | (w.height & 0xFFFF), ((x << 16) & 0xFFFF0000) | (y & 0xFFFF));
}
Window getWindow(uint64_t width, uint64_t height, HexColor *pixels)
{
    Window ans;
    ans.width = width;
    ans.height = height;
    ans.pixels = pixels;
    return ans;
}