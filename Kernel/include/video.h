#ifndef VDO_H
#define VDO_H

#include <stdint.h>

enum HEX_COLORS
{
	HEX_RED = 0xFF0000,
	HEX_GREEN = 0x00FF00,
	HEX_BLUE = 0x0000FF,
};

typedef uint32_t HexColor;
typedef HexColor (*ShapeFunction)(int, int);

void putPixel(HexColor hexColor, uint64_t x, uint64_t y);
void putPixelStd(uint8_t opacity, uint8_t red, uint8_t green, uint8_t blue, uint64_t x, uint64_t y);
void drawRectangle(HexColor hexColor, uint64_t x, uint64_t y, int width, int height);
/***
 * ShapeFunction receives coordinates (two integers), and returns a HexColor of form
 * 0xOORRGGBB. Where OO is the opacity for that pixel, and RRGGBB a hexColor
 *
 * @todo Opacity is not implemented yet
 */
void drawShape(ShapeFunction f, int x, int y, int xRange, int yRange);
void drawScaledShape(ShapeFunction f, int x, int y, int xRange, int yRange, double xScaleFactor, double yScaleFactor);

HexColor drawRedLine(int x, int y);
HexColor drawRedCircle(int x, int y);
HexColor drawAnEllipse(int x, int y);
HexColor drawACanvas(int x, int y);

#endif
