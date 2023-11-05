#ifndef VDO_H
#define VDO_H

#include <stdint.h>

/**
 * @brief A color in the format 0xAARRGGBB
 */
typedef uint32_t HexColor;
/**
 * @brief A function that receives coordinates and returns a HexColor
 *
 * @param x The x coordinate of the pixel
 * @param y The y coordinate of the pixel
 * @param w The width of the shape
 * @param h The height of the shape
 */
typedef HexColor (*ShapeFunction)(int x, int y, int w, int h);

/**
 * @brief Shorthand for
 * putPixelStd(GET_OPACITY(hexColor), GET_RED(hexColor), GET_GREEN(hexColor), GET_BLUE(hexColor), x, y);
 *
 * @param hexColor 0xAARRGGBB
 * @param x The x coordinate of the pixel
 * @param y The y coordinate of the pixel
 * @return int 1 on success, 0 if failed
 */
int putPixel(HexColor hexColor, uint64_t x, uint64_t y);
/**
 * @brief Puts a pixel in the screen
 *
 * @param opacity The opacity of the pixel
 * @param red The red component of the pixel
 * @param green The green component of the pixel
 * @param blue The blue component of the pixel
 * @param x The x coordinate of the pixel
 * @param y The y coordinate of the pixel
 * @return int 1 on success, 0 if failed
 */
int putPixelStd(uint8_t opacity, uint8_t red, uint8_t green, uint8_t blue, uint64_t x, uint64_t y);
/**
 * @brief The name says it all. For performance gains, it ignores the alpha channel
 *
 * @param array The ShapeFunction to draw to full screen
 * @return uint64_t The number of pixels drawn (width * height)
 */
uint64_t super_fast_fill_screen_shape(ShapeFunction f);
/**
 * @brief Draws a shape to the screen
 *
 * @param f The ShapeFunction to draw
 * @param width The width of the shape
 * @param height The height of the shape
 * @param x The x coordinate of the top-left corner
 * @param y The y coordinate of the top-left corner
 * @return uint64_t The number of pixels drawn
 */
uint64_t drawShape(ShapeFunction f, uint32_t width, uint32_t height, uint32_t x, uint32_t y);
/**
 * @brief Draws a scaled shape to the screen
 *
 * @param f The ShapeFunction to draw
 * @param width The width of the shape
 * @param height The height of the shape
 * @param x The x coordinate of the top-left corner
 * @param y The y coordinate of the top-left corner
 * @param xScaleFactor The factor to scale the width by
 * @param yScaleFactor The factor to scale the height by
 * @return uint64_t The number of pixels drawn
 */
uint64_t drawScaledShape(ShapeFunction f, uint32_t width, uint32_t height, uint32_t x, uint32_t y, double xScaleFactor, double yScaleFactor);
/**
 * @brief The name says it all. For performance gains, it ignores the alpha channel
 *
 * @param array The array to draw to full screen
 * @return uint64_t The number of pixels drawn (width * height)
 */
uint64_t super_fast_fill_screen_array(HexColor *array);
/**
 * @brief Receives an array, its dimensions, and the position to draw the top-left corner from
 *
 * @param array The array to draw
 * @param width The width of the array
 * @param height The height of the array
 * @param x The x coordinate of the top-left corner
 * @param y The y coordinate of the top-left corner
 * @return uint64_t The number of pixels drawn
 */
uint64_t drawFromArray(HexColor *array, uint32_t width, uint32_t height, uint32_t x, uint32_t y);
/**
 * @brief Hyper efficient way to clear the screen
 */
void clear_screen();
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

#endif
