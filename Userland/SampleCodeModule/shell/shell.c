#include "shell.h"
#include "commandHandler/commandHandler.h"
void warpLineUp(int lines);
void passCommand(char *toPass);
void paintLineStart();
static char *buffer, *commandBuffer;
static uint64_t index, commandIndex;
static HexColor letterColor = 0xFF000000 | HEX_WHITE, highlightColor = 0x00000000 | HEX_BLACK;
static char *lineStart = ":~ ";
char shellStart()
{
    uint32_t width = getScreenWidth();
    uint32_t height = getScreenHeight();
    index = 0;
    commandIndex = 0;

    char c;
    commandBuffer = malloc(sizeof(char) * (MAX_COMMAND_LENGTH + 1));
    buffer = malloc((width * height) / TRUE_LETTER_HEIGHT / TRUE_LETTER_WIDTH);
    initializeFonts();
    startPainter(width, height);
    setSize(1.0);
    paintString("You are now in shell:\n", letterColor, highlightColor);
    paintLineStart();
    char leaving = 0;
    setSize(1.0);
    setBackgroundArray(windowsArray);
    setBackgroundColorMap(windowsColorMap);
    setSnakeDrawing(BIG_DRAW_SIZE, goomba, goomba, goomba, goomba, marioItem, marioItemColorMap);
    setDrawOptions(1, 0, 0, 0);
    while ((c = getChar()) != '\n' || !strcmp(commandBuffer, "exit"))
    {
        if (c == '\b')
        {
            if (*(buffer - 1) == '\n' || !index)
                continue;
            else
            {
                index--;
                commandIndex--;
            }
        }
        else
        {
            buffer[index++] = c;
            if (c != '\n')
            {
                commandBuffer[commandIndex++] = c;
                if (commandIndex >= MAX_COMMAND_LENGTH)
                {
                    commandBuffer++;
                    commandIndex--;
                }
            }
            else
            {
                passCommand(commandBuffer);
                commandIndex = 0;
                *commandBuffer = 0;
                paintChar('\n', letterColor, highlightColor);
                paintLineStart();
                continue;
            }
        }
        commandBuffer[commandIndex] = 0;
        buffer[index] = 0;
        if (!paintChar(c, letterColor, highlightColor))
        {
            warpLineUp(1);
        }
    }
    blank();
    free(commandBuffer);
    free(buffer);
    return 1;
}

void warpLineUp(int lines)
{
    blank();
    for (int line = 0; line < lines; line++)
    {
        uint64_t lineLength = getCharPerLine();
        for (int i = 0; i < getCharPerLine && *buffer != '\n'; i++)
        {
            buffer++;
            index--;
        }
    }
    buffer++;
    index--; // get rid of that last '\n'
    paintString(buffer, letterColor, highlightColor);
}
void setLetterColor(HexColor color)
{
    letterColor = 0xFF000000 & color; // Letters cannot be transparent
}
void setHighlightColor(HexColor color)
{
    highlightColor = color; // highlights can be transparent
}
void resize(double size)
{
    setSize(size);
    blank();
    while (!paintString(buffer, letterColor, highlightColor))
        warpLineUp(1);
}
void passCommand(char *toPass)
{
    char mustRedraw = 0;
    char *toPaint = handleCommand(toPass, &mustRedraw);
    if (mustRedraw)
    {
        buffer[--index] = 0; // to remove last '\n' from buffer
        paintString("You are now in shell:\n", letterColor, highlightColor);
        paintString(buffer, letterColor, highlightColor);
    }
    if (!strcmp(toPaint, ""))
    {
        paintString("\n", letterColor, highlightColor);
        paintString(toPaint, letterColor, highlightColor);
    }
}
void paintLineStart()
{
    char *aux = lineStart;
    while (*aux)
    {
        paintChar(*aux, letterColor, highlightColor);
        buffer[index++] = *(aux++);
    }
}