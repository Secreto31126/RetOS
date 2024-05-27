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

typedef shellData *shell;

typedef struct shellData
{
    painter p;
    char *buffer, *commandBuffer;
    uint64_t index, commandIndex;
    HexColor letterColor, highlightColor;
    char fromLastEnter;
} shellData;

char shellStart(painter p);
void setHighlightColor(shell s, HexColor color);
void setLetterColor(shell s, HexColor color);
void resize(shell s, double size);
void warpNLines(shell s, uint64_t lines);
void clearShell(shell s);
void paintStringOrWarp(shell s, char *str, char ask);

#endif
