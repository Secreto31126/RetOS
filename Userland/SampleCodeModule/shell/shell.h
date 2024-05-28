#ifndef SHL_H
#define SHL_H
#include <stdint.h>
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
typedef struct shellData
{
    painter p;
    char *buffer, *commandBuffer;
    uint64_t index, commandIndex;
    HexColor letterColor, highlightColor;
    char fromLastEnter;
} shellData;
typedef shellData *shell;
typedef char *(*action_t)(shell, char *, char *);

typedef struct command
{
    char *code;
    char *help;
    action_t action;
} command;
typedef struct commandSet
{
    command *commands;
    int commandCount;
} commandSet;

char shellStart(painter p, commandSet comSet);
void setHighlightColor(shell s, HexColor color);
void setLetterColor(shell s, HexColor color);
void resize(shell s, double size);
void warpNLines(shell s, uint64_t lines);
void clearShell(shell s);
void paintStringOrWarp(shell s, char *str, char ask);

#endif
