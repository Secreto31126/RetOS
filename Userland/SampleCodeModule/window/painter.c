#include "painter.h"
#include <stdmem.h>
#define LINE_START_MAX 20
#define X_LINE_END (((int)(width / (size * TRUE_LETTER_WIDTH))) * size * TRUE_LETTER_WIDTH)
static double size = 1.0;
static uint64_t width, height, xPointer = 0, yPointer = TRUE_LETTER_HEIGHT;
static Window stamp;
static char justBlanked = 0;

void startPainter(uint64_t w, uint64_t h)
{
    width = w;
    height = h;
    stamp = getWindow(TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, malloc(TRUE_LETTER_HEIGHT * TRUE_LETTER_WIDTH * sizeof(HexColor)));
    if (stamp.pixels == NULL)
        exit(1);
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
    if (stamp.pixels == NULL)
        exit(1);
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
void drawCharOnWindow(Window w, char c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y);
void paintBackSpaceOnWindow(Window w)
{
    if (xPointer <= 0)
    {
        yPointer -= size * TRUE_LETTER_HEIGHT;
        xPointer = X_LINE_END;
    }
    xPointer -= TRUE_LETTER_WIDTH * size;
    drawCharOnWindow(w, 0, 0xFF000000, 0xFF000000, xPointer, yPointer);
}
void drawCharAt(char c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y)
{
    drawCharToWindow(stamp, c, letterColor, highlightColor);
    drawWindow(stamp, x, y);
}
void drawCharOnWindow(Window w, char c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y)
{
    drawCharToWindow(stamp, c, letterColor, highlightColor);
    drawFromHexArray(w, stamp.pixels, stamp.width, stamp.height, x, y, 1, 1);
}
char paintCharOnWindow(Window w, char c, HexColor letterColor, HexColor highlightColor)
{
    if (c == '\b')
    {
        if (!(xPointer <= 0 && yPointer <= 0)) // shouldn't be able to backspace through line starters, but shell should handle that.
        {
            paintBackSpaceOnWindow(w);
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

    if (c == '\t')
    {
        for (int i = 0; i < SPACES_IN_TAB; i++)
            paintCharOnWindow(w, ' ', letterColor, highlightColor);
    }
    else
    {
        drawCharOnWindow(w, c, letterColor, highlightColor, xPointer, yPointer);
        moveCursor();
    }
    return 1;
}
char paintChar(char c, HexColor letterColor, HexColor highlightColor)
{
    justBlanked = 0;
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

    if (c == '\t')
    {
        for (int i = 0; i < SPACES_IN_TAB; i++)
            paintChar(' ', letterColor, highlightColor);
    }
    else
    {
        drawCharAt(c, letterColor, highlightColor, xPointer, yPointer);
        moveCursor();
    }
    return 1;
}
char fastPaintString(const char *c, HexColor letterColor, HexColor highlightColor)
{
    if (!justBlanked)
        return 2;
    uint64_t h = getScreenHeight(), w = getScreenWidth();
    Window wholeScreen = getWindow(w, h, pshm(h * w * sizeof(HexColor))); // uses pshm, as the image does not fit in ~128 kb for memory assignation
    if (wholeScreen.pixels == NULL)
        return 2;
    justBlanked = 0;

    while (*c && paintCharOnWindow(wholeScreen, *c, letterColor, highlightColor))
    {
        c++;
    }
    quickDraw(wholeScreen);
    free_shm(wholeScreen.pixels);
    if (*c)
        return 0;
    return 1;
}
char paintString(const char *c, HexColor letterColor, HexColor highlightColor)
{
    char aux = fastPaintString(c, letterColor, highlightColor);
    if (!aux)
        return 0;
    if (aux == 1)
        return 1;
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
    justBlanked = 1;
    clear();
}
void endPainter()
{
    freeWindow(stamp);
}