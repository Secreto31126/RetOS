#include "fontInterface.h"
#include "fonts.h"
#define FONT_COUNT 1
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
    loadFont("Default", Default, 0);
    setFont(0);
}

char drawStringToWindow(Window w, char *string, HexColor letterColor, HexColor highlightColor, double size)
{
    int index = 0;
    double width = TRUE_LETTER_WIDTH * size;
    double height = TRUE_LETTER_HEIGHT * size;
    uint64_t drawLimitX = w.width - width;
    uint64_t drawLimitY = w.height - height;
    for (int i = 0; i < drawLimitY; i += height) // Whole window must be colored, as the window is not ensured to be empty (Otherwise, backspace and similar would not affect window)
        for (int j = 0; j < drawLimitX; j += width)
        {
            if (string[index])
            {
                char letter[TRUE_LETTER_HEIGHT][TRUE_LETTER_WIDTH] = currentFont.letters[string[index]]; // Surprisingly enough, this is correct
                HexColor aux[TRUE_LETTER_HEIGHT][TRUE_LETTER_WIDTH];
                drawFromHexArray(w, toHexArray(letter, aux, TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, 2, highlightColor, letterColor), TRUE_LETTER_WIDTH, TRUE_LETTER_HEIGHT, j, i, size, size); // Arte.
            }
        }
    if (string[index])
        return 0;
    return 1;
}