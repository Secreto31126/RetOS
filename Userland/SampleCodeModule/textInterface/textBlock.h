#ifndef TXT_H
#define TXT_H

#include "./../dataStructures/list.h"
#include "./../dataStructures/string.h"
#include "./../nstdlib/nstdlib.h"
#include "./../window/window.h"

typedef struct textBlockHeader
{
    list lines;
    uint64_t scale;
    uint64_t width;
    uint64_t height;
    HexColor highlightColor;
    HexColor textColor;
} textBlockHeader;

typedef textBlockHeader *textBlock;

textBlock getTextBlock(uint64_t width, uint64_t height);
void display(textBlock t);
void addChar(textBlock t, char c);
void addString(textBlock t, char *s);
void freeTextBlock(textBlock toFree);

#endif