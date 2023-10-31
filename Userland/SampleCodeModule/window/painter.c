#include "painter.h"
#define LINE_START_MAX 20
#define X_LINE_END (((int)(w / (size * TRUE_LETTER_WIDTH))) * size * TRUE_LETTER_WIDTH)
static double size = 1.0, maxLineSize = 1.0;
static uint64_t w, h, xPointer = 0, yPointer = 0;
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
    freeWindow(stamp);
    stamp.pixels = malloc(((int)(TRUE_LETTER_HEIGHT * size)) * ((int)(TRUE_LETTER_WIDTH * size)) * sizeof(HexColor));
    if (s > maxLineSize)
        maxLineSize = s;
}
void newLine()
{
    yPointer += TRUE_LETTER_HEIGHT * maxLineSize;
    xPointer = 0;
    maxLineSize = size;
}
void paintBackSpace()
{
    if (xPointer <= 0)
    {
        yPointer -= maxLineSize * TRUE_LETTER_HEIGHT;
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
        if ((yPointer + TRUE_LETTER_HEIGHT * maxLineSize * 2) > h) // *2 because letters are drawn from cursor downwards, so otherwise last line would have its top on the bottom of the screen
            return 0;
        else
            newLine();
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

uint64_t getCharPerLine()
{
    return (uint64_t)(w / TRUE_LETTER_WIDTH / size);
}

void blank()
{
    xPointer = 0;
    yPointer = 0;
    Window blanker = getWindow(w, h, malloc(w * h * sizeof(HexColor)));
    char blackPixel[1] = {0};
    toHexArray(blackPixel, blanker.pixels, 1, 1, w, h, 1, 0xFF000000);
    drawWindow(blanker, 0, 0);
    freeWindow(blanker);
}
void endPainter()
{
    freeWindow(stamp);
}