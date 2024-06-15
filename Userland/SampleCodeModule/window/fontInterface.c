#include "fontInterface.h"
#include "fonts.h"
#include "./../nstdlib/nstdlib.h"
#define FONT_COUNT 1
#define ASCII_MAX 128
typedef struct
{
    char *name;
    char *letters;
} font;

static font fonts[FONT_COUNT] = {{.name = "Default",
                                  .letters = (char *)Classic}};
static font *currentFont = fonts;
void setFont(int fontNumber) // was intended to allow changing fonts. Memory limitations prevented this
{
    currentFont = fonts; // The rest is commented because it wasn't ever implemented, and pvs insists
    /*
    if (FONT_COUNT > 1)
    {
        currentFont = fonts + (fontNumber % FONT_COUNT);
    }
    else
        currentFont = fonts;
        */
}

// Whole window will not be colored. Shell must handle backspace and whatnot (Literally just draw an ascii value that is all zeroes (such as '\b'), then reduce index).
// Char will be automatically scaled to fit the window
void drawCharToWindow(Window w, char c, HexColor letterColor, HexColor highlightColor)
{
    toHexArray((currentFont->letters) + ((int)c % ASCII_MAX) * TRUE_LETTER_HEIGHT * TRUE_LETTER_WIDTH, w.pixels, TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, w.width, w.height, 2, highlightColor, letterColor); // This is equivalent and quicker
}
