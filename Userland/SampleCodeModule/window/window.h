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
    OPAQUE = 0xFF000000,
    TRANSPARENT = 0x00000000,
};

typedef enum OVERLAY_MODE
{
    TRANSPARENT,
    OPAQUE,
} OVERLAY_MODE;

typedef uint32_t HexColor;
typedef HexColor (*ShapeFunction)(int, int, double, double); // First two are the x and y coordinates regarding the origin. The second two are scale factors of the shape

typedef struct Window
{
    uint64_t width;
    uint64_t height;
    HexColor *pixels;
} Window;

uint64_t screenWidth();
uint64_t screenHeight();
uint64_t drawWindow(Window w, uint64_t x, uint64_t y);
uint64_t clear();
HexColor *toHexArray(char *source, HexColor *result, int width, int height, int resultWidth, int resultHeight, int colorCount, ...);
void drawFromHexArray(Window w, HexColor *source, int sourceWidth, int sourceHeight, int x, int y, double xScaleFactor, double yScaleFactor);
uint64_t getScreenWidth();
uint64_t getScreenHeight();
Window getWindow(uint64_t width, uint64_t height, HexColor *pixels);
Window rotateBy90(Window w);
Window rotateBy180(Window w);
Window rotateBy270(Window w);
void freeWindow(Window w);
// Offsets refer to position of window top left corner regarding the 'origin' of the ShapeFunction used.
Window overlayOnWindow(Window w, ShapeFunction f, uint64_t xOffset, uint64_t yOffset, double xScaleFactor, double yScaleFactor, OVERLAY_MODE m);
Window overlayFromCharArray(Window w, char *source, uint64_t sourceWidth, uint64_t sourceHeight, HexColor *map, uint64_t xOffset, uint64_t yOffset, OVERLAY_MODE m);
Window fromCharArray(Window w, char *source, uint64_t sourceWidth, uint64_t sourceHeight, HexColor *map, uint64_t xOffset, uint64_t yOffset, OVERLAY_MODE m);
uint64_t quickDraw(Window w);
HexColor colorMapper(HexColor *colorMap, int code);

#endif
