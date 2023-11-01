#include "shell.h"
#include "commandHandler/commandHandler.h"
#define BLOCK 50
void warpLineUp(uint64_t lines);
void passCommand(char *toPass);
void paintLineStart();
void paintStringOrWarp(char *s);
void paintCharOrWarp(char c);
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
    paintString("You are now in shell:\n", letterColor, highlightColor);
    paintLineStart();
    char leaving = 0;
    while ((c = getChar()) != '\n' || !strcmp(commandBuffer, "exit"))
    {
        paintCharOrWarp(c);
        if (c == '\b')
        {
            if (*(buffer - 1) == '\n' || !index)
            {
                continue;
            }
            else
            {
                index--;
                commandIndex--;
                commandBuffer[commandIndex] = 0;
                buffer[index] = 0;
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

                // passCommand(commandBuffer);
                commandIndex = 0;
                *commandBuffer = 0;
                paintLineStart();
            }
        }
        commandBuffer[commandIndex] = 0;
        buffer[index] = 0;
    }
    blank();
    free(commandBuffer);
    free(buffer);
    freePrints();
    return 1;
}

void warpLineUp(uint64_t lines)
{
    blank();
    uint64_t len = strlen(buffer);
    uint64_t maxRemove = getCharPerLine();
    int i, j;
    for (i = 0; lines && i < len; i++)
    {
        if ((!(i % maxRemove) && i) || buffer[i] == '\n')
            lines--;
    }
    for (j = i; j < len; j++)
        buffer[j - i] = buffer[j];
    index -= i;
    buffer[index] = 0;
    paintString(buffer, letterColor, highlightColor);
}
void warpOneLine()
{
    blank();
    uint64_t max = getCharPerLine();
    int i, j;
    for (i = 0; i < max && buffer[i] && buffer[i] != '\n'; i++)
        ;
    i++;
    for (j = i; buffer[j]; j++)
        buffer[j - i] = buffer[j];
    index -= i;
    buffer[index] = 0;

    paintString(buffer, letterColor, highlightColor);
}
void setLetterColor(HexColor color)
{
    letterColor = 0xFF000000 | color; // Letters cannot be transparent
    blank();
    paintStringOrWarp(buffer);
}
void setHighlightColor(HexColor color)
{
    highlightColor = color; // highlights can be transparent
    blank();
    paintStringOrWarp(buffer);
}
void resize(double size)
{
    setSize(size);
    blank();
    paintStringOrWarp(buffer);
}
void passCommand(char *toPass)
{
    char mustRedraw = 0;
    char *toPaint = handleCommand(toPass, &mustRedraw);
    if (mustRedraw)
    {
        buffer[--index] = 0; // to remove last '\n' from buffer
        paintStringOrWarp("You are now in shell:\n");
        paintStringOrWarp(buffer);
    }
    else
    {
        char *aux = toPaint;
        while (*aux)
            buffer[index++] = *(aux++);
        buffer[index] = 0;
    }
    if (!strcmp(toPaint, ""))
    {
        if (!paintString(toPaint, letterColor, highlightColor))
        {
            blank();
            paintStringOrWarp(buffer);
        }
        buffer[index++] = '\n';
        buffer[index] = 0;
        if (!paintChar('\n', letterColor, highlightColor))
        {
            blank();
            paintStringOrWarp(buffer);
        }
    }
    freePrints(); // just out of some sense of responsibility (and because the return of handleCommand may be an allocated string)
}
void paintLineStart()
{
    paintStringOrWarp(lineStart);
    char *aux = lineStart;
    while (*aux)
    {
        buffer[index++] = *(aux++);
    }
}
void paintStringOrWarp(char *s)
{
    while (!paintString(s, letterColor, highlightColor))
        warpOneLine();
}
void paintCharOrWarp(char c)
{
    while (!paintChar(c, letterColor, highlightColor))
        warpOneLine();
}