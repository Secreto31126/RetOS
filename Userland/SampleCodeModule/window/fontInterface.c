#include "fontInterface.h"
#include "fonts.h"
#include "./../nstdlib/nstdlib.h"
#define FONT_COUNT 1
#define ASCII_MAX 128
typedef struct font
{
    char *name;
    char *letters;
} font;

static font fonts[FONT_COUNT];
static font currentFont;
void loadFont(char *fontName, char *fontLetters, int fontNumber)
{
    fonts[fontNumber % FONT_COUNT].name = fontName;
    fonts[fontNumber % FONT_COUNT].letters = fontLetters;
}
void setFont(int fontNumber)
{
    currentFont = fonts[fontNumber % FONT_COUNT];
}
void initialize()
{
    loadFont("Default", Classic, 0);
    setFont(0);
}

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
            printf("j:%dw.width:%ddrawLimitX:%d\t", (int)j, w.width, drawLimitX);
            if (string[index])
            {
                char *letter;
                *letter = currentFont.letters[string[index] % ASCII_MAX]; // Surprisingly enough, this is correct
                HexColor aux[TRUE_LETTER_HEIGHT][TRUE_LETTER_WIDTH];
                drawFromHexArray(w, toHexArray(letter, aux, TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, 2, highlightColor, letterColor), TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, (int)(j + 0.5), (int)(i + 0.5), size, size); // Arte.
                index++;
            }
        }
    if (string[index])
        return 0;
    return 1;
}