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

    if (!VBE_mode_info->bpp)
        return 0;

    framebuffer_element *framebuffer = FRAMEBUFFER;

    uint64_t offset = x + y * VBE_mode_info->pitch / (VBE_mode_info->bpp / 8);
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

uint64_t super_fast_fill_screen(HexColor *array)
{
    uint64_t size = VBE_mode_info->width * VBE_mode_info->height;
    framebuffer_element *end = (framebuffer_element *)((uint64_t *)((uint64_t)VBE_mode_info->framebuffer) + size);

    framebuffer_element *i = FRAMEBUFFER;
    while (i < end - 1)
    {
        *(i++) = *((framebuffer_element *)(array++));
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

    if (!x && !y && width >= VBE_mode_info->width && height >= VBE_mode_info->height)
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

    uint64_t *i = VBE_mode_info->framebuffer;
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
