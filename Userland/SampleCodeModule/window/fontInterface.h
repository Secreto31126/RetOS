#ifndef FNTI_H
#define FNTI_H
#include "window.h"
#include "fonts.h"
#include <stdint.h>

void drawCharToWindow(Window w, char c, HexColor letterColor, HexColor highlightColor); // Will fill a window with a drawing of the character given from fonts.c, stretched to the window's width and height
void setFont(int fontNumber);

#endif
