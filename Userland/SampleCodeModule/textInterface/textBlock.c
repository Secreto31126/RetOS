#include "textBlock.h"

textBlock getTextBlock(uint64_t width, uint64_t height)
{
    textBlock toRet = malloc(sizeof(textBlockHeader));
    toRet->height = height;
    toRet->width = width;
    toRet->scale = 1;
    toRet->highlightColor = HEX_BLACK | OPAQUE;
    toRet->textColor = HEX_WHITE | OPAQUE;
    toRet->lines = createList();
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