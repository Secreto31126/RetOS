#include "painter.h"
#define LINE_START_MAX 20
#define X_LINE_END(p) (((int)((p)->w / ((p)->size * TRUE_LETTER_WIDTH))) * (p)->size * TRUE_LETTER_WIDTH)

painter startPainter(uint64_t width, uint64_t height)
{
    painter p = malloc(sizeof(painterHeader));
    p->xPointer = 0;
    p->yPointer = TRUE_LETTER_HEIGHT;
    p->size = 1.0;
    p->w = width;
    p->h = height;
    p->stamp = getWindow(TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, malloc(TRUE_LETTER_HEIGHT * TRUE_LETTER_WIDTH * sizeof(HexColor)));
    return p;
}
void setSize(painter p, double s)
{
    if (s < 0.49 || s > 9.0)
        return;
    p->size = s;
    p->stamp.width = TRUE_LETTER_WIDTH * p->size;
    p->stamp.height = TRUE_LETTER_HEIGHT * p->size;
    free(p->stamp.pixels);
    p->stamp.pixels = malloc(((int)(TRUE_LETTER_HEIGHT * p->size)) * ((int)(TRUE_LETTER_WIDTH * p->size)) * sizeof(HexColor)); // realloc unnecessary, as all characters are only temporarily stamped
}
uint64_t getSize(painter p)
{
    return (uint64_t)(p->size + 0.5);
}
void newLine(painter p)
{
    p->yPointer += TRUE_LETTER_HEIGHT * p->size;
    p->xPointer = 0; // xPointer leaves room for header
}
void moveCursor(painter p)
{
    p->xPointer += TRUE_LETTER_WIDTH * p->size;
}
void paintBackSpace(painter p)
{
    if (p->xPointer <= 0)
    {
        p->yPointer -= p->size * TRUE_LETTER_HEIGHT;
        p->xPointer = X_LINE_END(p);
    }
    drawCharToWindow(p->stamp, 0, 0xFF000000, 0xFF000000);
    p->xPointer -= TRUE_LETTER_WIDTH * p->size;
    drawWindow(p->stamp, p->xPointer, p->yPointer);
}
void drawCharAt(painter p, char c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y)
{
    drawCharToWindow(p->stamp, c, letterColor, highlightColor);
    drawWindow(p->stamp, x, y);
}
char paintChar(painter p, char c, HexColor letterColor, HexColor highlightColor)
{
    if (c == '\b')
    {
        if (!(p->xPointer <= 0 && p->yPointer <= 0)) // shouldn't be able to backspace through line starters, but shell should handle that.
        {
            paintBackSpace(p);
        }
        return 1;
    }
    if ((p->xPointer + TRUE_LETTER_WIDTH * p->size) > p->w || c == '\n')
    {
        if ((p->yPointer + TRUE_LETTER_HEIGHT * p->size * 2) > p->h) // *2 because letters are drawn from cursor downwards and to the right, so otherwise last line would have its top on the bottom of the screen
            return 0;
        else
        {
            newLine(p);
            if (c == '\n')
                return 1;
        }
    }
    drawCharAt(p, c, letterColor, highlightColor, p->xPointer, p->yPointer);
    moveCursor(p);
    return 1;
}
char paintString(painter p, const char *c, HexColor letterColor, HexColor highlightColor)
{
    while (*c && paintChar(p, *c, letterColor, highlightColor))
    {
        c++;
    }
    if (*c)
        return 0;
    return 1;
}
// This implementation is not beautiful modularization, it is simple, easy and doesn't repeat code instead.
void drawStringAt(painter p, char *c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y)
{
    uint64_t auxX = p->xPointer, auxY = p->yPointer;
    p->xPointer = x;
    p->yPointer = y;
    paintString(p, c, letterColor, highlightColor);
    p->xPointer = auxX;
    p->yPointer = auxY;
}
uint64_t maxYPointer(painter p)
{
    return p->h - 2 * p->size * TRUE_LETTER_HEIGHT;
}
uint64_t maxXPointer(painter p)
{
    return p->w - TRUE_LETTER_WIDTH * p->size;
}
char willFit(painter p, const char *s)
{
    double xP = 0, yP = 0, maxX = p->w - TRUE_LETTER_WIDTH * p->size, maxY = p->h - 2 * TRUE_LETTER_HEIGHT * p->size;
    while (*s)
    {
        if (*s == '\n' || xP > maxX)
        {
            xP = 0;
            yP += TRUE_LETTER_HEIGHT * p->size;
            if (yP > maxY)
                return 0;
        }
        else
        {
            xP += TRUE_LETTER_WIDTH * p->size;
        }
        s++;
    }
    return 1;
}

uint64_t getCharPerLine(painter p)
{
    return (uint64_t)(p->w / TRUE_LETTER_WIDTH / p->size);
}

void blankNoClear(painter p)
{
    p->xPointer = 0;
    p->yPointer = TRUE_LETTER_HEIGHT * p->size;
}
void blank(painter p)
{
    blankNoClear(p);
    clear();
}
void endPainter(painter p)
{
    freeWindow(p->stamp);
}