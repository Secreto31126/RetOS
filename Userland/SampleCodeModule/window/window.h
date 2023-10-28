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
    FIT_TO_SCREEN_L_R,
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER_L_R
} L_R_ALIGNMENT;
typedef enum T_B_ALIGNMENT
{
    FIT_TO_SCREEN_T_B,
    ALIGN_TOP,
    ALIGN_BOTTOM,
    ALIGN_CENTER_T_B
} T_B_ALIGNMENT;

typedef struct Window
{
    uint64_t width;
    uint64_t height;
    HexColor *pixels;
} Window;

uint64_t screenWidth();
uint64_t screenHeight();
HexColor *toHexArray(char *source, HexColor *result, int width, int height, int resultWidth, int resultHeight, int colorCount, ...);
void drawFromHexArray(Window w, HexColor *source, int sourceWidth, int sourceHeight, int x, int y, double xScaleFactor, double yScaleFactor);
uint64_t getScreenWidth();
uint64_t getScreenHeight();
uint64_t drawWindow(Window w, uint64_t x, uint64_t y);
Window getWindow(uint64_t width, uint64_t height, HexColor *pixels);
Window rotateBy90(Window w);

#endif
