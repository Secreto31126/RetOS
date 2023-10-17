#ifndef VDO_H
#define VDO_H

#include <stdint.h>

enum HEX_COLORS
{
	HEX_RED = 0xFF0000,
	HEX_GREEN = 0x00FF00,
	HEX_BLUE = 0x0000FF,
};

typedef uint32_t (*ShapeFunction)(int, int);

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void putPixelStd(char red, char green, char blue, uint64_t x, uint64_t y);
void drawRectangle(uint32_t hexColor, uint64_t x, uint64_t y, int width, int height);
/***
 * ShapeFunction receives coordinates (two integers), and returns a uint32_t of form
 * 0xPPRRGGBB. Where PP is boolean for whether to draw that pixel, and RRGGBB a hexColor
 */
void drawShape(ShapeFunction f, int x, int y, int xRange, int yRange);
void drawScaledShape(ShapeFunction f, int x, int y, int xRange, int yRange, double xScaleFactor, double yScaleFactor);

#endif
