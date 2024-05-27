#ifndef PNT_H
#define PNT_H
#include "window.h"
#include "fontInterface.h"
#include "./../nstdlib/nstdlib.h"
// Blank function automatically clears screen. For this reason, each process still should handle only one instance of painter
// Use blankNoClear(p) and clear() from here if you want multiple painters per process (still not recommended)
typedef painterHeader *painter;
typedef struct painterHeader
{
    double size;
    uint64_t w, h, xPointer, yPointer;
    Window stamp;
} painterHeader;

painter startPainter(uint64_t width, uint64_t height);

// empties out buffer, clears screen
void blank(painter p);
// empties out buffer
void blankNoClear(painter p);
// returns 0 if out of space, 1 otherwise
char paintChar(painter p, char c, HexColor letterColor, HexColor highlightColor);
// returns 0 if did not fit. 1 otherwise. String is still painted partially if can't fit. Reccomended action is call to blank(), then paint again.
char paintString(painter p, const char *c, HexColor letterColor, HexColor highlightColor);
// draw the given character, top left corner on (x;y)
void drawCharAt(painter p, char c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y);
void drawStringAt(painter p, char *c, HexColor letterColor, HexColor highlightColor, uint64_t x, uint64_t y);
void setSize(painter p, double s);
void endPainter(painter p);
uint64_t getCharPerLine(painter p);
char willFit(painter p, const char *s);
uint64_t getSize(painter p);

#endif
