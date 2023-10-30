#include "window.h"
#include "painter.h"
#include "figures.h"
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
    double xScaleFactor = (double)sourceWidth / resultWidth, yScaleFactor = (double)sourceHeight / resultHeight;
    for (int c = 0; c < colorCount; c++)
    {
        replacement = va_arg(colors, HexColor);
        int xIndex, yIndex = 0;
        for (double i = 0; i < sourceHeight && yIndex < resultHeight; i += yScaleFactor)
        {
            xIndex = 0;
            for (double j = 0; j < sourceWidth && xIndex < resultWidth; j += xScaleFactor)
            {
                if (source[(int)(j) + (int)(i)*sourceWidth] == c)
                    result[xIndex + yIndex * resultWidth] = replacement;
                xIndex++;
            }
            yIndex++;
        }
    }
    va_end(colors);
    return result;
}
void swap(Window w, int i1, int j1, int i2, int j2)
{
    HexColor aux;
    aux = w.pixels[i1 * w.width + j1];
    w.pixels[i1 * w.width + j1] = w.pixels[i2 * w.width + j2];
    w.pixels[i2 * w.width + j2] = aux;
}
Window rotateBy90(Window w)
{
    int width = w.width, height = w.height;
    double ratio = (double)width / height;
    HexColor *source = w.pixels;
    HexColor *result = malloc(width * height * sizeof(HexColor));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            result[i * width + j] = source[((int)((width - j - 1) / ratio)) * width + ((int)(i * ratio))];
        }
    }
    int size = height * width;
    for (int i = 0; i < size; i++)
    {
        source[i] = result[i];
    }
    free(result);
    return w;
}
Window rotateBy270(Window w) // more efficient that three 90 rotations
{
    int width = w.width, height = w.height;
    double ratio = (double)width / height;
    HexColor *source = w.pixels;
    HexColor *result = malloc(width * height * sizeof(HexColor));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            result[i * width + j] = source[((int)(j / ratio)) * width + width - ((int)(i * ratio))];
        }
    }
    int size = height * width;
    for (int i = 0; i < size; i++)
    {
        source[i] = result[i];
    }
    free(result);
    return w;
}
Window rotateBy180(Window w)
{
    int hOver2 = w.height / 2, width = w.width, height = w.height;
    for (int i = 0; i < hOver2; i++)
    {
        for (int j = 0; j < width; j++)
        {
            swap(w, i, j, height - i - 1, j);
        }
    }
    return w;
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

void freeWindow(Window w)
{
    free(w.pixels);
}

Window overlayOnWindow(Window w, ShapeFunction f, uint64_t xOffset, uint64_t yOffset, double xScaleFactor, double yScaleFactor, OVERLAY_MODE m)
{
    HexColor *toOverlay = w.pixels; // To avoid referencing within a loop
    uint64_t height = w.height, width = w.width;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (m == OPAQUE)
                toOverlay[i * width + j] = f(j + xOffset, i + yOffset, xScaleFactor, yScaleFactor);
            else
                toOverlay[i * width + j] = mergeColor(toOverlay[i * width + j], f(j + xOffset, i + yOffset, xScaleFactor, yScaleFactor));
        }
    }
    return w;
}

Window overlayFromCharArray(Window w, char *source, uint64_t sourceWidth, uint64_t sourceHeight, HexColor *map, uint64_t xOffset, uint64_t yOffset, OVERLAY_MODE m)
{
    uint64_t width = w.width, height = w.height, screenW = getScreenWidth(), screenH = getScreenHeight();
    HexColor *result = w.pixels;
    double xScaleFactor = (double)sourceWidth / screenW, yScaleFactor = (double)sourceHeight / screenH;
    int xIndex, yIndex = 0;
    for (double i = yOffset * sourceHeight / screenH; i < sourceHeight && yIndex < height; i += yScaleFactor)
    {
        xIndex = 0;
        for (double j = xOffset * sourceWidth / screenW; j < sourceWidth && xIndex < width; j += xScaleFactor)
        {
            if (m == OPAQUE)
                result[xIndex + yIndex * width] = colorMapper(map, source[(int)(j) + (int)(i)*sourceWidth]);
            else
                result[xIndex + yIndex * width] = mergeColor(result[xIndex + yIndex * width], colorMapper(map, source[(int)(j) + (int)(i)*sourceWidth]));
            xIndex++;
        }
        yIndex++;
    }
    return w;
}