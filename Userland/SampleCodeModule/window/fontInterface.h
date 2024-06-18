#ifndef FNTI_H
#define FNTI_H
#include "fonts.h"
#include "window.h"
#include <stdint.h>

char drawStringToWindow(
    Window w, char *string, HexColor letterColor, HexColor highlightColor,
    double size); // Will draw to the window from blank. return 0 if ran out of
                  // space, 1 otherwise.
void drawCharToWindow(
    Window w, char c, HexColor letterColor,
    HexColor highlightColor); // Will fill a window with a drawing of the
                              // character given from fonts.c, stretched to the
                              // window's width and height
void setFont(int fontNumber);

#endif
