#include "video.h"

VBEInfo *VBE_mode_info = (VBEInfo *)0x5C00; // hardcoded
#define FRAMEBUFFER ((framebuffer_element *)((uint64_t)VBE_mode_info->framebuffer))

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
    // Gotta go fast
    if (!opacity)
        return 1;

    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height)
        return 0;

    framebuffer_element *framebuffer = FRAMEBUFFER;

    uint64_t offset = x + y * VBE_mode_info->width;
    framebuffer_element old_pixel = framebuffer[offset];
    framebuffer_element new_pixel = {
        .b = mergeColor(old_pixel.b, blue, opacity),
        .g = mergeColor(old_pixel.g, green, opacity),
        .r = mergeColor(old_pixel.r, red, opacity),
    };

    framebuffer[offset] = new_pixel;
    return 1;
}

static uint8_t mergeColor(uint8_t background, uint8_t overlay, uint8_t opacity)
{
    if (!opacity)
        return background;
    double op = (double)opacity / 0xFF;
    return (background * (1 - op) + overlay * op);
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

uint64_t super_fast_fill_screen(HexColor *array)
{
    uint64_t width = VBE_mode_info->width;
    uint64_t height = VBE_mode_info->height;
    uint64_t size = width * height;

    if (!size)
        return 0;

    uint64_t output = 0;
    uint8_t tracker = 0;
    uint64_t *writer = (uint64_t *)FRAMEBUFFER;

    uint64_t i = 0;
    uint64_t mod = size % 8;
    while (i < size - mod)
    {
        uint32_t color = (uint32_t)(array[i++]);

        // Revert the pixel order and save them in the output
        for (uint64_t j = 0; j < 3; j++)
        {
            output <<= 8;
            output += color & 0xFF;
            color >>= 8;

            if (++tracker >= 8)
            {
                // *writer++ = output;
                // Wut?
                ((char *)writer)[0] = (output >> 56) & 0xFF;
                ((char *)writer)[1] = (output >> 48) & 0xFF;
                ((char *)writer)[2] = (output >> 40) & 0xFF;
                ((char *)writer)[3] = (output >> 32) & 0xFF;
                ((char *)writer)[4] = (output >> 24) & 0xFF;
                ((char *)writer)[5] = (output >> 16) & 0xFF;
                ((char *)writer)[6] = (output >> 8) & 0xFF;
                ((char *)writer)[7] = (output >> 0) & 0xFF;
                writer++;
                tracker = 0;
                output = 0;
            }
        }
    }

    while (i < size)
    {
        HexColor color = array[i++];
        putPixelStd(-1, GET_RED(color), GET_GREEN(color), GET_BLUE(color), i % width, i / height);
    }

    return size;
}

uint64_t drawFromArray(HexColor *array, uint32_t width, uint32_t height, uint32_t x, uint32_t y)
{
    if (!array)
    {
        clear_screen();
        return VBE_mode_info->width * VBE_mode_info->height;
    }

    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height)
        return 0;

    if (!x && !y && width > VBE_mode_info->width && height > VBE_mode_info->height)
        return super_fast_fill_screen(array);

    uint64_t drawn = 0;
    for (uint32_t i = 0; i < height && i < VBE_mode_info->height; i++)
    {
        for (uint32_t j = 0; j < width && j < VBE_mode_info->width; j++)
        {
            drawn += putPixel(array[j + i * width], x + j, y + i);
        }
    }

    return drawn;
}

void clear_screen()
{
    uint64_t size = VBE_mode_info->width * VBE_mode_info->height;
    uint64_t *end = (uint64_t *)(FRAMEBUFFER + size);

    if (!size)
        return;

    uint64_t *i = (uint64_t *)FRAMEBUFFER;
    while (i < end - 1)
    {
        *(i++) = 0;
    }

    *(end - 1) = 0; // hackerman
}

uint16_t get_width()
{
    return VBE_mode_info->width;
}

uint16_t get_height()
{
    return VBE_mode_info->height;
}
