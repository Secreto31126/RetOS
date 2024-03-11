#ifndef TXT_H
#define TXT_H

#include "./../dataStructures/list.h"
#include "./../dataStructures/string.h"
#include "./../nstdlib/nstdlib.h"

typedef struct textBlockHeader
{

} textBlockHeader;

typedef textBlockHeader *textBlock;

void initializeTextDisplay();
int getTextBlock();
void freeTextBlock(textBlock toFree);

#endif