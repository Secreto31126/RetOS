#include "textBlock.h"
#include "./../window/fonts.h"

static HexColor *stampPixels[TRUE_LETTER_HEIGHT * TRUE_LETTER_WIDTH];
void putCharAt(char c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y);
void drawTextBlock(textBlock t);

textBlock getTextBlock(uint64_t width, uint64_t height)
{
    textBlock toRet = malloc(sizeof(textBlockHeader));
    toRet->height = height;
    toRet->width = width;
    toRet->scale = 1;
    toRet->highlightColor = HEX_BLACK | OPAQUE;
    toRet->textColor = HEX_WHITE | OPAQUE;
    toRet->lines = createList();
    add(toRet->lines, createString());
    return toRet;
}

void freeTextBlock(textBlock toFree)
{
    if (toFree != null)
    {
        free(toFree->lines);
        free(toFree);
    }
}

void display(textBlock t);

char willCauseLineBreak(textBlock t, char c)
{
    return c == '\n' || t->width < (listSize(peek(t->lines)) + 1) * t->scale * TRUE_LETTER_WIDTH;
}
char noSpaceOnBottom(textBlock t)
{
    return t->height < (listSize(t->lines) + 1) * t->scale * TRUE_LETTER_HEIGHT;
}

void putChar(textBlock t, char c)
{
    string last = peek(t->lines);
    if (c == '\b')
    {
        if (!stringSize(peek(t->lines)))
        {
            if (listSize(t->lines) > 1)
                remove(t->lines, listSize(t->lines) - 1);
        }
        else
        {
            putCharAt(' ', t->textColor, t->highlightColor, stringSize(last) * TRUE_LETTER_WIDTH, listSize(t->lines) * TRUE_LETTER_HEIGHT);
            backspace(peek(t->lines));
        }
        return;
    }

    if (!willCauseLineBreak(t, c))
    {
        addChar(last, c);
        putCharAt(c, t->textColor, t->highlightColor, stringSize(last) * TRUE_LETTER_WIDTH, listSize(t->lines) * TRUE_LETTER_HEIGHT);
        return;
    }

    if (noSpaceOnBottom(t))
    {
        remove(t->lines, 0);
        clear();
        drawTextBlock(t);
    }
    string newLine = createString();
    addChar(newLine, c);
    add(t->lines, newLine);
    if (c != '\n')
        putCharAt(c, t->textColor, t->highlightColor, stringSize(last) * TRUE_LETTER_WIDTH, listSize(t->lines) * TRUE_LETTER_HEIGHT);
}

void drawTextBlock(textBlock t)
{
    listIterator lIter = getListIterator(t->lines);
    int x = 0, y = 0;
    while (hasNextL(lIter))
    {
        stringIterator sIter = next(lIter);
        while (hasNextS(sIter))
        {
            putCharAt(nextS(sIter), t->textColor, t->highlightColor, x, y);
            x += TRUE_LETTER_WIDTH;
        }
        y += TRUE_LETTER_HEIGHT;
        free(sIter);
    }
    free(lIter); // The true garbage collector
}

void putCharAt(char c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y)
{
    Window stamp = getWindow(TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, stampPixels);
    drawCharToWindow(stamp, c, letterColor, highlightColor);
    drawWindow(stamp, x, y);
}

void putString(textBlock t, char *s)
{
    while (*s)
        putChar(t, *s++);
}