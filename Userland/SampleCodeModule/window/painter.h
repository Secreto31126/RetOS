#ifndef PNT_H
#define PNT_H
#include "window.h"
#include "fontInterface.h"
#include "./../nstdlib/nstdlib.h"

void startPainter(uint64_t width, uint64_t height);

// empties out buffer, clears screen
void blank();
// returns 0 if out of space, 1 otherwise
char paintChar(char c, HexColor letterColor, HexColor highlightColor);
// returns 0 if did not fit. 1 otherwise. String is still painted partially if can't fit. Reccomended action is call to blank(), then paint again.
char paintString(char *c, HexColor letterColor, HexColor highlightColor);
void setSize(double s);
void endPainter();
uint64_t getCharPerLine();
char willFit(char *s);
void quickBlank();

#endif
