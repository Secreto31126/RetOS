#ifndef SHL_H
#define SHL_H
#include <stdint.h>
#include "shell.h"
#include "./../nstdlib/nstdlib.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"
#include "./../window/painter.h"
#include "./../snake/random.h"
#include "./../snake/snake.h"
#include "./../snake/drawings/backgroundArrays.h"
#include "./../snake/drawings/snakeDrawings.h"
#include "./../piano/piano.h"

#define MAX_COMMAND_LENGTH 90

char shellStart();
void setHighlightColor(HexColor color);
void setLetterColor(HexColor color);
void resize(double size);
void warpNLines(uint64_t lines);
void clearShell();

#endif
