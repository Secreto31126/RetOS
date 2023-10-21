#include "video.h"

VBEInfo *VBE_mode_info = (VBEInfo *)0x5C00; // hardcoded

#define GET_HEX(a, r, g, b) (((a) << 24) + ((r) << 16) + ((g) << 8) + (b))
#define GET_OPACITY(x) (((x) >> 24) & 0xFF)
#define GET_RED(x) (((x) >> 16) & 0xFF)
#define GET_GREEN(x) (((x) >> 8) & 0xFF)
#define GET_BLUE(x) ((x) & 0xFF)

int putPixel(HexColor hexColor, uint64_t x, uint64_t y)
{
    return putPixelStd(GET_OPACITY(hexColor), GET_RED(hexColor), GET_GREEN(hexColor), GET_BLUE(hexColor), x, y);
}

int putPixelStd(uint8_t opacity, uint8_t red, uint8_t green, uint8_t blue, uint64_t x, uint64_t y)
{
    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height)
        return 0;

    // Gota go fast
    if (!opacity)
    {
        return 1;
    }

    uint8_t *framebuffer = (uint8_t *)VBE_mode_info->framebuffer; // tira warning. Pasas que pasan
    // En el array, cada pixel tiene tres valores (RGB), por lo que la posición en el array es x*3
    uint64_t offset = x * (VBE_mode_info->bpp / 8) + y * VBE_mode_info->pitch; // es 3 lo primero, la cantidad de líneas (y) por el tamaño de una línea lo segundo
    framebuffer[offset] = mergeColor(framebuffer[offset], blue, opacity);
    framebuffer[offset + 1] = mergeColor(framebuffer[offset + 1], green, opacity);
    framebuffer[offset + 2] = mergeColor(framebuffer[offset + 2], red, opacity);

    return 1;
}

uint8_t mergeColor(uint8_t background, uint8_t overlay, uint8_t opacity)
{
    if (!opacity)
        return background;
    double op = (double)opacity / 0xFF;
    return (background * (1 - op) + overlay * op) / 2;
}

void drawShape(ShapeFunction f, int x, int y, int xRange, int yRange)
{
    drawScaledShape(f, x, y, xRange, yRange, 1, 1);
}

void drawScaledShape(ShapeFunction f, int x, int y, int xRange, int yRange, double xScaleFactor, double yScaleFactor)
{
    if (!xScaleFactor || !yScaleFactor)
        return;

    for (int i = 0; i < yRange && i < VBE_mode_info->height; i++)
    {
        for (int j = 0; j < xRange && j < VBE_mode_info->width; j++)
        {
            uint32_t r = f(j / xScaleFactor, i / yScaleFactor, xRange, yRange);
            if (r & 0xFF000000)
                putPixel(r, x + j, y + i);
        }
    }
}

uint64_t drawFromArray(HexColor *array, uint32_t width, uint32_t height, uint32_t x, uint32_t y)
{
    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height)
        return 0;

    uint64_t drawn = 0;
    for (uint32_t i = 0; i < height && i < VBE_mode_info->height; i++)
    {
        for (uint32_t j = 0; j < width && j < VBE_mode_info->width; j++)
        {
            drawn += putPixel(array[j + i * height], x + j, y + i);
        }
    }

    return drawn;
}

uint16_t get_width()
{
    return VBE_mode_info->width;
}

uint16_t get_height()
{
    return VBE_mode_info->height;
}
