#include "painter.h"
#define LINE_START_MAX 20
#define X_LINE_END (((int)(width / (size * TRUE_LETTER_WIDTH))) * size * TRUE_LETTER_WIDTH)
static double size = 1.0;
static uint64_t width, height, xPointer = 0, yPointer = TRUE_LETTER_HEIGHT;
static Window stamp;

void startPainter(uint64_t w, uint64_t h)
{
    width = w;
    height = h;
    stamp = getWindow(TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, malloc(TRUE_LETTER_HEIGHT * TRUE_LETTER_WIDTH * sizeof(HexColor)));
}
void setSize(double s)
{
    if (s < 0.49 || s > 9.0)
        return;
    size = s;
    stamp.width = TRUE_LETTER_WIDTH * size;
    stamp.height = TRUE_LETTER_HEIGHT * size;
    free(stamp.pixels);
    stamp.pixels = malloc(((int)(TRUE_LETTER_HEIGHT * size)) * ((int)(TRUE_LETTER_WIDTH * size)) * sizeof(HexColor)); // realloc unnecessary, as all characters are only temporarily stamped
}
uint64_t getSize()
{
    return (uint64_t)(size + 0.5);
}
void newLine()
{
    yPointer += TRUE_LETTER_HEIGHT * size;
    xPointer = 0; // xPointer leaves room for header
}
void moveCursor()
{
    xPointer += TRUE_LETTER_WIDTH * size;
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
void drawCharAt(char c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y)
{
    drawCharToWindow(stamp, c, letterColor, highlightColor);
    drawWindow(stamp, x, y);
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
    if ((xPointer + TRUE_LETTER_WIDTH * size) > width || c == '\n')
    {
        if ((yPointer + TRUE_LETTER_HEIGHT * size * 2) > height) // *2 because letters are drawn from cursor downwards and to the right, so otherwise last line would have its top on the bottom of the screen
            return 0;
        else
        {
            newLine();
            if (c == '\n')
                return 1;
        }
    }
    drawCharAt(c, letterColor, highlightColor, xPointer, yPointer);
    moveCursor();
    return 1;
}
char paintString(const char *c, HexColor letterColor, HexColor highlightColor)
{
    while (*c && paintChar(*c, letterColor, highlightColor))
    {
        c++;
    }
    if (*c)
        return 0;
    return 1;
}
// This implementation is not beautiful modularization, it is simple, easy and doesn't repeat code instead.
void drawStringAt(char *c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y)
{
    uint64_t auxX = xPointer, auxY = yPointer;
    xPointer = x;
    yPointer = y;
    paintString(c, letterColor, highlightColor);
    xPointer = auxX;
    yPointer = auxY;
}
uint64_t maxYPointer()
{
    return height - 2 * size * TRUE_LETTER_HEIGHT;
}
uint64_t maxXPointer()
{
    return width - TRUE_LETTER_WIDTH * size;
}
char willFit(const char *s)
{
    double xP = 0, yP = 0, maxX = width - TRUE_LETTER_WIDTH * size, maxY = height - 2 * TRUE_LETTER_HEIGHT * size;
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
    return (uint64_t)(width / TRUE_LETTER_WIDTH / size);
}

void blank()
{
    xPointer = 0;
    yPointer = TRUE_LETTER_HEIGHT * size;
    clear();
}
void endPainter()
{
    freeWindow(stamp);
}