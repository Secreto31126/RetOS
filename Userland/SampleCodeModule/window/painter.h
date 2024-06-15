#ifndef PNT_H
#define PNT_H
#include "window.h"
#include "fontInterface.h"
#include "./../nstdlib/nstdlib.h"
#define SPACES_IN_TAB 4

/**
 * @brief Initialize the painter
 *
 * @param width The width of the screen, in pixels
 * @param height Thei height of the screen, in pixels
 */
void startPainter(uint64_t width, uint64_t height);

/**
 * @brief Moves cursor to the start, clears screen
 *
 */
void blank();
/**
 * @brief Paints a character on the cursor, moves the cursor
 *
 * @param c The character to paint
 * @param letterColor
 * @param highlightColor
 * @return char 0 if the character fit, 1 otherwise
 */
char paintChar(char c, HexColor letterColor, HexColor highlightColor);
/**
 * @brief Paints a string starting on the cursor, moves the cursor
 *
 * @note String is still painted partially if can't fit. Recommended action is call to blank(), then paint again.
 *
 * @param s The string to paint
 * @param letterColor
 * @param highlightColor
 * @return char 0 if the string fit, 1 otherwise
 */
char paintString(const char *s, HexColor letterColor, HexColor highlightColor);
/**
 * @brief draw the given character
 *
 * @note top left of the character is drawn on (x;y)
 *
 * @param c The character to paint
 * @param letterColor
 * @param highlightColor
 * @param x x-coordinate for the character
 * @param y y-coordinate for the character
 */
void drawCharAt(char c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y);
/**
 * @brief draw the given string
 *
 * @note top left of the string is drawn on (x;y)
 *
 * @param c The character to paint
 * @param letterColor
 * @param highlightColor
 * @param x x-coordinate for the string
 * @param y y-coordinate for the string
 */
void drawStringAt(char *c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y);
/**
 * @brief Set the size of characters drawn by the painter
 *
 * @param s The new size
 */
void setSize(double s);
/**
 * @brief Free all memory allocated by the painter
 *
 */
void endPainter();
/**
 * @brief Get the number of characters that fit in a line
 *
 * @return uint64_t
 */
uint64_t getCharPerLine();
/**
 * @brief Find whether a string will fit in the screen given the cursor's current position
 *
 * @param s The string
 * @return char 0 if the character will not fit, 1 otherwise
 */
char willFit(const char *s);
/**
 * @brief Get the current size of characters printed by the painter
 *
 * @return uint64_t
 */
uint64_t getSize();

#endif
