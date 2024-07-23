#ifndef SHL_H
#define SHL_H
#include "./../window/fontInterface.h"
#include "./../window/window.h"
#include "shell.h"
#include <stdint.h>

#define MAX_COMMAND_LENGTH 900

char shellStart();
void setHighlightColor(HexColor color);
void setLetterColor(HexColor color);
void resize(double size);
void warpNLines(uint64_t lines);
void clearShell();
void paintStringOrWarp(const char *s, char ask);
void addStringToBuffer(const char *s, char ask);

#endif
