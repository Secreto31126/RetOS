#ifndef WND_H
#include <stdint.h>
#define WND_H

/**
 * @brief List of most common colors
 * in 0xRRGGBB format
 */
enum HEX_COLORS
{
    HEX_RED = 0xFF0000,
    HEX_GREEN = 0x00FF00,
    HEX_BLUE = 0x0000FF,
};

typedef uint32_t HexColor;
typedef HexColor (*ShapeFunction)(int, int, int, int);

typedef enum L_R_ALIGNMENT
{
    FIT_TO_SCREEN,
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER
} L_R_ALIGNMENT;
typedef enum T_B_ALIGNMENT
{
    FIT_TO_SCREEN,
    ALIGN_TOP,
    ALIGN_BOTTOM,
    ALIGN_CENTER
} L_R_ALIGNMENT;

typedef struct Window
{
    uint64_t width;
    uint64_t height;
    HexColor *pixels;
} Window;

uint64_t screenWidth();
uint64_t screenHeight();

#endif
