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
    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height)
        return;

    uint8_t *framebuffer = (uint8_t *)VBE_mode_info->framebuffer; // tira warning. Pasas que pasan
    // En el array, cada pixel tiene tres valores (RGB), por lo que la posición en el array es x*3
    uint64_t offset = x * (VBE_mode_info->bpp / 8) + y * VBE_mode_info->pitch; // es 3 lo primero, la cantidad de líneas (y) por el tamaño de una línea lo segundo
    framebuffer[offset] = blue;
    framebuffer[offset + 1] = green;
    framebuffer[offset + 2] = red;
}

void drawShape(ShapeFunction f, int x, int y, int xRange, int yRange)
{
    drawScaledShape(f, x, y, xRange, yRange, 1, 1);
}

void drawScaledShape(ShapeFunction f, int x, int y, int xRange, int yRange, double xScaleFactor, double yScaleFactor)
{
    if (!xScaleFactor || !yScaleFactor)
        return;

    for (int i = 0; i < xRange; i++)
    {
        for (int j = 0; j < yRange; j++)
        {
            uint32_t r = f(i / xScaleFactor, j / yScaleFactor);
            if (r & 0xFF000000)
                putPixel(r & 0x00FFFFFF, i + x, j + y);
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
