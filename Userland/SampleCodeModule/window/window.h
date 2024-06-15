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

typedef struct Window
{
    uint64_t width;
    uint64_t height;
    HexColor *pixels;
} Window;

/**
 * @brief The width of the screen, in pixels
 *
 * @return uint64_t
 */
uint64_t getScreenWidth();
/**
 * @brief The height of the screen, in pixels
 *
 * @return uint64_t
 */
uint64_t getScreenHeight();
/**
 * @brief Draws the content of a Window on screen at the given coordinates
 *
 * @param w The Window
 * @param x Coordinate of the left edge of the drawing
 * @param y Coordinate of the top edge of the drawing
 * @return uint64_t The number of pixels drawn
 */
uint64_t drawWindow(Window w, uint64_t x, uint64_t y);
/**
 * @brief Clears the screen
 *
 * @return uint64_t The number of pixels on the screen
 */
uint64_t clear();
/**
 * @brief Converts an array of char to an array of HexColor, via a colormap
 *
 * @note Automatically scales source to result.
 * To make a color in the result be 'transparent' in later operations, set its HexColor opacity to 0.
 * This function is for translating bitmap-equivalents to HexColor arrays. Not for creating Windows.
 * Chars set to 0 will be copied to result as color 1, set to 1 copied as color 2, etc.
 *
 * @param source The array of char
 * @param result Return variable, where the HexColor array should be created
 * @param width The width of the source array
 * @param height The height of the source array
 * @param resultWidth The width of the result array
 * @param resultHeight The height of the result array
 * @param colorCount The number of colors present in the soure array
 * @param ... The mapping of char to HexColor
 * @return HexColor* The result parameter
 */
HexColor *toHexArray(char *source, HexColor *result, int width, int height, int resultWidth, int resultHeight, int colorCount, ...);
/**
 * @brief Draws a HexColor array onto a given coordinate on a Window, scaling it
 *
 * @param w The window
 * @param source The HexColor array
 * @param sourceWidth The width of the HexColor array
 * @param sourceHeight The height of the HexColor array
 * @param x The x coordinate of the left edge of the HexColor array
 * @param y The y coordinate of the top edge of the HexColor array
 * @param xScaleFactor
 * @param yScaleFactor
 */
void drawFromHexArray(Window w, HexColor *source, int sourceWidth, int sourceHeight, int x, int y, double xScaleFactor, double yScaleFactor);
/**
 * @brief Get a Window
 *
 * @param width The width of the pixels parameter
 * @param height The height of the pixels parameter
 * @param pixels An allocated HexColor array
 * @return Window
 */
Window getWindow(uint64_t width, uint64_t height, HexColor *pixels);
/**
 * @brief Rotate the contents of a Window by 90 degrees
 *
 * @param w The Window
 * @return Window
 */
Window rotateBy90(Window w);
/**
 * @brief Rotate the contents of a Window by 180 degrees
 *
 * @param w The Window
 * @return Window
 */
Window rotateBy180(Window w);
/**
 * @brief Rotate the contents of a Window by 270 degrees
 *
 * @param w The Window
 * @return Window
 */
Window rotateBy270(Window w);
/**
 * @brief Free the contents of a Window
 *
 * @param w The Window
 */
void freeWindow(Window w);
/**
 * @brief Maps a portion of a char array onto HexColor array on a window, having resized the char array to be the size of the screen
 *
 * @param w The Window
 * @param source The char array
 * @param sourceWidth The true width of the char array
 * @param sourceHeight The true height of the char array
 * @param map The color mapping from char to HexColor
 * @param xOffset The x coordinate on the screen corresponding to the left edge of the snippet to be mapped
 * @param yOffset The y cooridnate on the screen corresponding to the top edge of the snippet to be mapped
 * @param m Whether transparency values should be considered
 * @return Window
 */
Window overlayFromCharArray(Window w, char *source, uint64_t sourceWidth, uint64_t sourceHeight, HexColor *map, uint64_t xOffset, uint64_t yOffset, OVERLAY_MODE m);
/**
 * @brief Maps a portion of a char array onto HexColor array on a Window, resizing it to occupy the whole Window
 *
 * @param w The Window
 * @param source The char array
 * @param sourceWidth The true width of the char array
 * @param sourceHeight The true height of the char array
 * @param map The color mapping from char to HexColor
 * @param m Whether transparency values should be considered
 * @return Window
 */
Window fromCharArray(Window w, char *source, uint64_t sourceWidth, uint64_t sourceHeight, HexColor *map, OVERLAY_MODE m);
/**
 * @brief Draws a window the size of the whole screen via a faster method
 *
 * @note Will default to drawWindow function if the screen is not the size of the whole screen
 *
 * @param w
 * @return uint64_t
 */
uint64_t quickDraw(Window w);
/**
 * @brief Obtain the HexColor corresponding to a code given a colormap
 *
 * @param colorMap
 * @param code
 * @return HexColor
 */
HexColor colorMapper(HexColor *colorMap, int code);
/**
 * @brief Overlays an image given by a ShapeFunction on a Window
 *
 * @note Offsets refer to position of window top left corner regarding the 'origin' of the ShapeFunction used.
 * @deprecated ShapeFunctions no longer used in this interface
 *
 * @param w
 * @param f
 * @param xOffset
 * @param yOffset
 * @param xScaleFactor
 * @param yScaleFactor
 * @param m
 * @return Window
 */
Window overlayOnWindow(Window w, ShapeFunction f, uint64_t xOffset, uint64_t yOffset, double xScaleFactor, double yScaleFactor, OVERLAY_MODE m);

#endif
