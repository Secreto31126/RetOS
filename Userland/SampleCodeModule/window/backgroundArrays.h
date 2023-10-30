#ifndef BCKG_H
#define BCKG_H
#define BACKGROUND_WIDTH 102
#define BACKGROUND_HEIGHT 77
#define APPLE_BROWN 0xFF452C09
#define APPLE_RED 0xFFba2709
#define APPLE_GREEN 0xFF124F0D
#include "window.h"

extern HexColor appleColorMap[];
extern HexColor windowsColorMap[];
extern char windowsArray[BACKGROUND_HEIGHT][BACKGROUND_WIDTH];
extern char marioArray[BACKGROUND_HEIGHT][BACKGROUND_WIDTH];
extern HexColor marioColorMap[];
extern HexColor pongColorMap[];
extern char pongArray[BACKGROUND_HEIGHT][BACKGROUND_WIDTH];

#endif
