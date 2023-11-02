#include "shell.h"
#include "commandHandler/commandHandler.h"
#define BLOCK 50
void warpNLines(uint64_t n);
void warpAndRedraw();
void warpOneLine();
char *passCommand(char *toPass);
void paintLineStart();
void paintStringOrWarp(char *s);
void paintCharOrWarp(char c);
static char *buffer, *commandBuffer;
static uint64_t index, commandIndex;
static HexColor letterColor = 0xFF000000 | HEX_WHITE, highlightColor = 0x00000000 | HEX_BLACK;
static const char *lineStart = ":~ ";
static const char *shellIntro = "You are now in shell:\n";
static char fromLastEnter = 0;
char shellStart()
{
    uint32_t width = getScreenWidth();
    uint32_t height = getScreenHeight();

    index = 0;
    commandIndex = 0;
    commandBuffer = malloc(sizeof(char) * (MAX_COMMAND_LENGTH + 1));
    buffer = malloc((width * height) / TRUE_LETTER_HEIGHT / TRUE_LETTER_WIDTH);
    *buffer = 0;
    *commandBuffer = 0;
    index += sPuts(buffer, shellIntro);
    paintString(buffer, letterColor, highlightColor);
    paintLineStart();

    char c;
    while ((c = getChar()) != '\n' || !strcmp(commandBuffer, "exit"))
    {
        if (c == '\b')
        {
            if (!fromLastEnter || !index)
            {
                continue;
            }
            else
            {
                paintChar(c, letterColor, highlightColor);
                index--;
                commandIndex--;
                fromLastEnter--;
            }
        }
        else
        {
            if (c == '\n')
            {
                fromLastEnter = 0;
                addStringToBuffer(passCommand(commandBuffer));
                freePrints();
                commandIndex = 0;
            }
            else
            {
                addCharToBuffer(c);
                fromLastEnter++;
                if (commandIndex < MAX_COMMAND_LENGTH)
                    commandBuffer[commandIndex++] = c; // ignores commands after a certain length (since no commands should be that long)
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

void addCharToBuffer(char c)
{
    buffer[index++] = c;
    buffer[index] = 0;
    paintCharOrWarp(c);
}
void addStringToBuffer(char *s)
{
    //  paintString(s, 0xffff0000, 0xff00ff00);
    char *aux = s;
    while (*aux)
        buffer[index++] = *(aux++);
    buffer[index] = 0;
    paintStringOrWarp(s);
}
void paintStringOrWarp(char *s)
{
    if (strcmp(s, ""))
        return;
    if (!willFit(buffer))
    {
        warpOneLine();
        while (!willFit(buffer))
            warpOneLine();
        blank();
        paintString(buffer, letterColor, highlightColor);
    }
    else
        paintString(s, letterColor, highlightColor);
}
void paintCharOrWarp(char c)
{
    if (!paintChar(c, letterColor, highlightColor))
        warpAndRedraw();
}

char *passCommand(char *toPass)
{
    char mustRedraw = 0;
    char *toPaint = handleCommand(toPass, &mustRedraw);

    if (mustRedraw)
    {
        char *toReturn = sPrintf("%s\n%s\n%s", buffer, toPaint, lineStart);
        index = 0;
        buffer[index] = 0;
        if (willFit(toReturn))
            blank();
        return toReturn;
    }
    if (strcmp(toPaint, ""))
        return sPrintf("\n%s", lineStart);
    return sPrintf("\n%s\n%s", toPaint, lineStart);
}
void warpOneLine()
{
    warpNLines(1);
}
void warpAndRedraw()
{
    warpOneLine();
    blank();
    paintString(buffer, letterColor, highlightColor);
}
void setLetterColor(HexColor color) // command handler is responsible for setting mustRedraw to 1
{
    letterColor = 0xFF000000 | color; // Letters cannot be transparent
}
void setHighlightColor(HexColor color) // command handler is responsible for setting mustRedraw to 1
{
    highlightColor = color; // highlights can be transparent
}
void resize(double size) // command handler is responsible for setting mustRedraw to 1
{
    setSize(size);
}

void clearShell()
{
    blank();
    index = 0;
    buffer[index] = 0;
    commandIndex = 0;
    commandBuffer[commandIndex] = 0;
    fromLastEnter = 0;
}

/**
 * @deprecated
 */
void warpNLines(uint64_t n) // char or char* you want to add must be in buffer already. This shortens the buffer from the start so that it fits, then repaints it.
{
    if (!n)
        return;
    uint64_t max = getCharPerLine();
    int i = 0, j, k = 0;
    while (n)
    {
        for (i = 0; i < max && buffer[k + i] && buffer[k + i] != '\n'; i++)
            ;
        n--;
        i++;
        k += i;
    }
    for (j = k; buffer[j]; j++)
        buffer[j - k] = buffer[j];
    index = j - k;
    buffer[index] = 0;
}
void paintLineStart()
{
    paintString(lineStart, letterColor, highlightColor);
    char *aux = lineStart;
    while (*aux)
    {
        buffer[index++] = *(aux++);
    }
    buffer[index] = 0;
}
