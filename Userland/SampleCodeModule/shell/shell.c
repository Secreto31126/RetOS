#include "shell.h"
void warpLineUp(int lines);

static char *buffer, *commandBuffer;
static uint64_t index, commandIndex;
static HexColor letterColor = 0xFF000000 | HEX_WHITE, highlightColor = 0x00000000 | HEX_BLACK;
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
    setLineStart(":~");
    paintString("You are now in shell:\n", letterColor, highlightColor);
    char leaving = 0;
    setSize(1.0);
    setBackgroundArray(creationArray);
    setBackgroundColorMap(creationColorMap);
    while ((c = getChar()) != 'q' || !leaving)
    {
        if (c == 'q')
            leaving = 1;
        else
            leaving = 0;
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
                commandIndex = 0;
                if (!strcmp(commandBuffer, "snake"))
                {
                    setSeed(get_tick());
                    blank();
                    char winner = playSnake(2);
                    paintString("You are now in shell. Player ", letterColor, highlightColor);
                    paintChar(winner + '0', letterColor, highlightColor);
                    paintString(" won", letterColor, highlightColor);
                }
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
void setPrompt(char *newPrompt)
{
    setLineStart(newPrompt);
    blank();
    paintString(buffer, letterColor, highlightColor);
}