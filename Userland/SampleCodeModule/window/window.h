#ifndef WND_H
#include <stdint.h>
#include "./../nstdlib/nstdlib.h"
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
    HEX_WHITE = 0xFFFFFF,
    HEX_BLACK = 0x000000,
};

typedef enum OVERLAY_MODE
{
    TRANSPARENT,
    OPAQUE,
} OVERLAY_MODE;

typedef uint32_t HexColor;
typedef HexColor (*ShapeFunction)(int, int, double, double); // First two are the x and y coordinates regarding the origin. The second two are scale factors of the shape

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
Window rotateBy180(Window w);
Window rotateBy270(Window w);
void freeWindow(Window w);
// Offsets refer to position of window top left corner regarding the 'origin' of the ShapeFunction used.
Window overlayOnWindow(Window w, ShapeFunction f, uint64_t xOffset, uint64_t yOffset, double xScaleFactor, double yScaleFactor, OVERLAY_MODE m);
Window overlayFromCharArray(Window w, char *source, uint64_t sourceWidth, uint64_t sourceHeight, HexColor *map, uint64_t xOffset, uint64_t yOffset, OVERLAY_MODE m);

#endif
