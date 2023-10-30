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

#define MAX_COMMAND_LENGTH 90

char shellStart();
void setHighlightColor(HexColor color);
void setLetterColor(HexColor color);
void resize(double size);
void setPrompt(char *newPrompt);
void warpLineUp(int lines);

#endif
