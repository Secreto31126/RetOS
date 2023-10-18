#ifndef VDO_H
#define VDO_H

#include <stdint.h>

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
typedef HexColor (*ShapeFunction)(int, int);

/**
 * @brief Shorthand for
 * putPixelStd(GET_OPACITY(hexColor), GET_RED(hexColor), GET_GREEN(hexColor), GET_BLUE(hexColor), x, y);
 *
 * @param hexColor 0xAARRGGBB
 * @param x
 * @param y
 */
void putPixel(HexColor hexColor, uint64_t x, uint64_t y);
/**
 * @brief Puts a pixel in the screen
 *
 * @param opacity The opacity of the pixel
 * @param red The red component of the pixel
 * @param green The green component of the pixel
 * @param blue The blue component of the pixel
 * @param x The x coordinate of the pixel
 * @param y The y coordinate of the pixel
 */
void putPixelStd(uint8_t opacity, uint8_t red, uint8_t green, uint8_t blue, uint64_t x, uint64_t y);
/***
 * ShapeFunction receives coordinates (two integers), and returns a HexColor of form
 * 0xOORRGGBB. Where OO is the opacity for that pixel, and RRGGBB a hexColor
 *
 * @todo Opacity is not implemented yet
 */
void drawShape(ShapeFunction f, int x, int y, int xRange, int yRange);
void drawScaledShape(ShapeFunction f, int x, int y, int xRange, int yRange, double xScaleFactor, double yScaleFactor);
/**
 * @brief Get the width size
 *
 * @return uint16_t 1024
 */
uint16_t get_width();
/**
 * @brief Get the height size
 *
 * @return uint16_t 768
 */
uint16_t get_height();

HexColor redRectangleFigure(int x, int y);
HexColor greenRectangleFigure(int x, int y);
HexColor blueRectangleFigure(int x, int y);
HexColor redLine(int x, int y);
HexColor redCircle(int x, int y);
/**
 * @deprecated
 */
HexColor drawEllipse(int x, int y);
HexColor RetOSbackground(int x, int y);

#endif
