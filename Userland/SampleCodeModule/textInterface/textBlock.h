#ifndef TXT_H
#define TXT_H

#include "./../dataStructures/list.h"
#include "./../dataStructures/string.h"
#include "./../nstdlib/nstdlib.h"
#include "./../window/window.h"

typedef struct textBlockHeader
{
    list lines;
    int scale;
    HexColor highlightColor;
    HexColor textColor;
} textBlockHeader;

typedef textBlockHeader *textBlock;

void initializeTextDisplay();
int getTextBlock();
void freeTextBlock(textBlock toFree);

#endif