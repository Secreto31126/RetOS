#include "painter.h"
#define LINE_START_MAX 20
#define X_LINE_END (((int)(w / (size * TRUE_LETTER_WIDTH))) * size * TRUE_LETTER_WIDTH)
static double size = 1.0;
static uint64_t w, h, xPointer = 0, yPointer = 0, xToErase = 0, yToErase = 0;
static Window stamp;

void startPainter(uint64_t width, uint64_t height)
{
    w = width;
    h = height;
    stamp = getWindow(TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, malloc(TRUE_LETTER_HEIGHT * TRUE_LETTER_WIDTH * sizeof(HexColor)));
}
void setSize(double s)
{
    size = s;
    stamp.width = TRUE_LETTER_WIDTH * size;
    stamp.height = TRUE_LETTER_HEIGHT * size;
    free(stamp.pixels);
    stamp.pixels = malloc(((int)(TRUE_LETTER_HEIGHT * size)) * ((int)(TRUE_LETTER_WIDTH * size)) * sizeof(HexColor));
}
void newLine()
{
    yPointer += TRUE_LETTER_HEIGHT * size;
    if (yPointer > yToErase)
        yToErase = yPointer;
    xPointer = 0;
}
void paintBackSpace()
{
    if (xPointer <= 0)
    {
        yPointer -= size * TRUE_LETTER_HEIGHT;
        xPointer = X_LINE_END;
    }
    drawCharToWindow(stamp, 0, 0xFF000000, 0xFF000000);
    xPointer -= TRUE_LETTER_WIDTH * size;
    drawWindow(stamp, xPointer, yPointer);
}
void drawChar(char c, HexColor letterColor, HexColor highlightColor)
{
    drawCharToWindow(stamp, c, letterColor, highlightColor);
    drawWindow(stamp, xPointer, yPointer);
    xPointer += TRUE_LETTER_WIDTH * size;
    if (xPointer > xToErase)
        xToErase = xPointer;
}
char paintChar(char c, HexColor letterColor, HexColor highlightColor)
{
    if (c == '\b')
    {
        if (!(xPointer <= 0 && yPointer <= 0)) // shouldn't be able to backspace through line starters, but shell should handle that.
        {
            paintBackSpace();
        }
        return 1;
    }
    if ((xPointer + TRUE_LETTER_WIDTH * size) > w || c == '\n')
        if ((yPointer + TRUE_LETTER_HEIGHT * size * 2) > h) // *2 because letters are drawn from cursor downwards, so otherwise last line would have its top on the bottom of the screen
            return 0;
        else
        {
            newLine();
            return 1;
        }
    drawChar(c, letterColor, highlightColor);
    return 1;
}
char paintString(char *c, HexColor letterColor, HexColor highlightColor)
{
    while (*c && paintChar(*c, letterColor, highlightColor))
    {
        c++;
    }
    if (*c)
        return 0;
    return 1;
}
uint64_t maxYPointer()
{
    return h - 2 * size * TRUE_LETTER_HEIGHT;
}
uint64_t maxXPointer()
{
    return w - TRUE_LETTER_WIDTH * size;
}
char willFit(char *s)
{
    double xP = 0, yP = 0, maxX = w - TRUE_LETTER_WIDTH * size, maxY = h - 2 * TRUE_LETTER_HEIGHT * size;
    while (*s)
    {
        if (*s == '\n' || xP > maxX)
        {
            xP = 0;
            yP += TRUE_LETTER_HEIGHT * size;
            if (yP > maxY)
                return 0;
        }
        else
        {
            xP += TRUE_LETTER_WIDTH * size;
        }
        s++;
    }
    return 1;
}

uint64_t getCharPerLine()
{
    return (uint64_t)(w / TRUE_LETTER_WIDTH / size);
}

void blank()
{
    xPointer = 0;
    yPointer = 0;
    clear();
}
// Only use if the only input to screen has been the painter
/**
 * @deprecated
 * clearing the whole screen via kernel is now significantly faster
 */
void quickBlank()
{
    xToErase += TRUE_LETTER_WIDTH * size;
    yToErase += TRUE_LETTER_HEIGHT * size;
    Window blanker = getWindow(xToErase, yToErase, malloc(xToErase * yToErase * sizeof(HexColor)));
    char blackPixel = 0;
    toHexArray(&blackPixel, blanker.pixels, 1, 1, xToErase, yToErase, 1, 0xFF000000);
    drawWindow(blanker, 0, 0);
    free(blanker.pixels);
    yToErase = 0;
    xToErase = 0;
    xPointer = 0;
    yPointer = 0;
}
void endPainter()
{
    freeWindow(stamp);
}