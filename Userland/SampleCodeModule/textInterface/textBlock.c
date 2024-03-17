#include "textBlock.h"
#include "./../window/fonts.h"

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
    return c == '\n' || t->width < (peek(t->lines)->size + 1) * t->scale * TRUE_LETTER_WIDTH;
}
char noSpaceOnBottom(textBlock t)
{
    return t->height < (t->lines->size + 1) * t->scale * TRUE_LETTER_HEIGHT;
}

void putChar(textBlock t, char c)
{
    string last = peek(t->lines);
    if (!willCauseLineBreak(t, c))
    {
        addChar(last, c);
        // paintCharAt
    }
    else
    {
        if (noSpaceOnBottom(t))
        {
            remove(t->lines, 0);
            // rePaintAll
        }
        string newLine = createString();
        addChar(newLine, c);
        add(t->lines, newLine);
        if (c != '\n')
        {
            // paintCharAt
        }
    }
}

void putString(textBlock t, char *s)
{
    while (*s)
    {
    }
}