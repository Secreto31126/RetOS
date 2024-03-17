#ifndef TXT_H
#define TXT_H

// Tambien conocido como el fragmentador-de-memoria-inador
// Es una lista de strings que se auto-genera nuevas lineas al quedarse sin espacio

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
void putChar(textBlock t, char c);
void putString(textBlock t, char *s);
void freeTextBlock(textBlock toFree);

#endif