#ifndef FNTI_H
#define FNTI_H
#include "window.h"
#include "fonts.h"
#include <stdint.h>

char drawStringToWindow(Window w, char *string, HexColor letterColor, HexColor highlightColor, double size); // Will draw to the window from blank. return 0 if ran out of space, 1 otherwise.
void drawCharToWindow(Window w, char c, HexColor letterColor, HexColor highlightColor);
char **getFonts();           // returns an array of all font names as strings. Termination array has pointer null.
char isFont(char *fontName); // returns 1 if the fontName is known, 0 otherwise.
void initializeFonts();

#endif
