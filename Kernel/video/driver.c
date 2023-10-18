#include "video.h"

VBEInfo *VBE_mode_info = (VBEInfo *)0x5C00; // hardcoded

#define GET_HEX(a, r, g, b) (((a) << 24) + ((r) << 16) + ((g) << 8) + (b))
#define GET_OPACITY(x) (((x) >> 24) & 0xFF)
#define GET_RED(x) (((x) >> 16) & 0xFF)
#define GET_GREEN(x) (((x) >> 8) & 0xFF)
#define GET_BLUE(x) ((x) & 0xFF)

void putPixel(HexColor hexColor, uint64_t x, uint64_t y)
{
    putPixelStd(GET_OPACITY(hexColor), GET_RED(hexColor), GET_GREEN(hexColor), GET_BLUE(hexColor), x, y);
}

void putPixelStd(uint8_t opacity, uint8_t red, uint8_t green, uint8_t blue, uint64_t x, uint64_t y)
{
    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height || !opacity)
        return;

    uint8_t *framebuffer = (uint8_t *)VBE_mode_info->framebuffer; // tira warning. Pasas que pasan
    // En el array, cada pixel tiene tres valores (RGB), por lo que la posición en el array es x*3
    uint64_t offset = x * (VBE_mode_info->bpp / 8) + y * VBE_mode_info->pitch; // es 3 lo primero, la cantidad de líneas (y) por el tamaño de una línea lo segundo
    framebuffer[offset] = mergeColor(framebuffer[offset], blue, opacity);
    framebuffer[offset + 1] = mergeColor(framebuffer[offset + 1], green, opacity);
    framebuffer[offset + 2] = mergeColor(framebuffer[offset + 2], red, opacity);
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

    for (int i = 0; i < xRange && i < VBE_mode_info->width; i++)
    {
        for (int j = 0; j < yRange && j < VBE_mode_info->height; j++)
        {
            uint32_t r = f(i / xScaleFactor, j / yScaleFactor);
            if (r & 0xFF000000)
                putPixel(r, i + x, j + y);
        }
    }
}

uint16_t get_width()
{
    return VBE_mode_info->width;
}

uint16_t get_height()
{
    return VBE_mode_info->height;
}

void drawFromArray(HexColor *array, uint64_t position, L_R_ALIGNMENT alignLR, T_B_ALIGNMENT alignTB)
{
    uint32_t width = (position >> 16) & 0xFFFF;
    uint32_t height = (position) & 0xFFFF;
    double xStep = alignLR == FIT_SCREEN ? ((double)width / get_width()) : 1.0;
    double yStep = alignTB == FIT_SCREEN ? ((double)height / get_height()) : 1.0;
    double start, end, xStep = 1.0, yStep = 1.0;
    switch (alignLR)
    {
    }
    switch (alignTB)
    {
    }
}
