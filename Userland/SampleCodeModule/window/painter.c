#include "painter.h"

static double size = 1.0, maxLineSize = 1.0;
char aux[1] = {0};
static char *lineStart = aux;
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
    if (strlen(lineStart) * TRUE_LETTER_WIDTH * size < w) // lineStart cannot occupy more than a line
    {
        size = s;
        if (s > maxLineSize)
            maxLineSize = s;
    }
}
void setLineStart(char *start)
{
    if (strlen(lineStart) * TRUE_LETTER_WIDTH * size < w)
        lineStart = start;
}
void newLine()
{
    yPointer += TRUE_LETTER_HEIGHT * maxLineSize;
    xPointer = 0;
    maxLineSize = size;
}
char paintChar(char c, HexColor letterColor, HexColor highlightColor)
{
    if (c == '\b')
    {
        if (xPointer <= 0) // currently can't backspace linebreaks. Also shouldn't be able to backspace through line starters, but shell should handle that.
            return 1;
        else
        {
            drawCharToWindow(stamp, 0, 0xFF000000, 0xFF000000);
            xPointer -= TRUE_LETTER_WIDTH * size;
            drawWindow(stamp, xPointer, yPointer);
        }
    }
    if ((xPointer + TRUE_LETTER_WIDTH * size) > w || c == '\n')
        if ((yPointer + TRUE_LETTER_HEIGHT * maxLineSize) > h)
            return 0;
        else
        {
            newLine();
            if (c == '\n')
                paintString(lineStart, letterColor, highlightColor);
        }
    drawCharToWindow(stamp, c, letterColor, highlightColor);
    drawWindow(stamp, xPointer, yPointer);
    xPointer += TRUE_LETTER_WIDTH * size;
    return 1;
}
char paintString(char *c, HexColor letterColor, HexColor highlightColor)
{
    while (*c && paintChar(c, letterColor, highlightColor))
    {
        c++;
    }
    if (*c)
        return 0;
    return 1;
}
void blank()
{
    xPointer = 0;
    yPointer = 0;
    while (paintChar(0, 0xFF000000, 0xFF000000)) // paints a fully black character on the whole window until the window is full
        ;
    xPointer = 0;
    yPointer = 0;
}
void endPainter()
{
    free(stamp.pixels);
}