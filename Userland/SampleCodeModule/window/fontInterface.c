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

static font fonts[FONT_COUNT];
static font *currentFont;
void loadFont(char *fontName, char *fontLetters, int fontNumber)
{
    fonts[fontNumber % FONT_COUNT].name = fontName;
    fonts[fontNumber % FONT_COUNT].letters = fontLetters;
}
void setFont(int fontNumber)
{
    currentFont = fonts + (fontNumber % FONT_COUNT);
}
void initialize()
{
    loadFont("Default", Classic, 0);
    setFont(0);
}
/**
 * @deprecated drawCharToWindow has more finesse (and can carry a paint bucket)
 */
char drawStringToWindow(Window w, char *string, HexColor letterColor, HexColor highlightColor, double size)
{
    int index = 0;
    double width = TRUE_LETTER_WIDTH * size;
    double height = TRUE_LETTER_HEIGHT * size;
    uint64_t drawLimitX = w.width - width;
    uint64_t drawLimitY = w.height - height;
    for (double i = 0; i < drawLimitY; i += height) // Whole window must be colored, as the window is not ensured to be empty (Otherwise, backspace and similar would not affect window)
        for (double j = 0; j < drawLimitX; j += width)
        {
            if (string[index])
            {
                char *letter;
                letter = (currentFont->letters) + ((int)string[index] % ASCII_MAX) * TRUE_LETTER_HEIGHT * TRUE_LETTER_WIDTH; // Surprisingly enough, this is correct
                HexColor aux[TRUE_LETTER_HEIGHT][TRUE_LETTER_WIDTH];
                drawFromHexArray(w, toHexArray(letter, aux, TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, 2, highlightColor, letterColor), TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, (int)(j + 0.5), (int)(i + 0.5), size, size); // Arte.
                index++;
            }
            else
            {
                // If we had bothered to finish this function, it would fill the rest of the window in black. We did not. We also did not erase it, it is a decent reference.
            }
        }
    if (string[index])
        return 0;
    return 1;
}

// Whole window will not be colored. Shell must handle backspace and whatnot (Literally just draw an ascii value that is all zeroes (such as '\b'), then reduce index).
// Char will be automatically scaled to fit the window, this works weirdly thus far, but probably won't in the future :D.
void drawCharToWindow(Window w, char c, HexColor letterColor, HexColor highlightColor)
{
    HexColor aux[TRUE_LETTER_HEIGHT][TRUE_LETTER_WIDTH];
    drawFromHexArray(w, toHexArray((currentFont->letters) + ((int)c % ASCII_MAX) * TRUE_LETTER_HEIGHT * TRUE_LETTER_WIDTH, aux, TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, 2, highlightColor, letterColor), TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, 0, 0, (double)w.width / TRUE_LETTER_WIDTH, (double)w.width / TRUE_LETTER_HEIGHT); // Variables auxiliares? Legibilidad? Couldn't be me.
}
