#include "shell.h"
#include "commandHandler/commandHandler.h"
#include <stdint.h>
#define BLOCK 50
#define MOVE_BY 8
void warpNLines(uint64_t n);
void warpAndRedraw();
void warpOneLine();
char *passCommand(char *toPass);
void paintLineStart();
void paintCharOrWarp(char c);
void paintStringOrWarp(char *s, char ask);
void addStringToBuffer(char *s, char ask);
void addCharToBuffer(char c);
static char *buffer, *commandBuffer;
static uint64_t index, commandIndex;
static HexColor letterColor = 0xFF000000 | HEX_WHITE, highlightColor = 0xFF000000 | HEX_BLACK;
static const char *lineStart = ":~ ";
static const char *shellIntro = "You are now in shell:\n";
static char fromLastEnter = 0;
void drawTime()
{
    drawStringAt(getTimeString(), 0xFFFFFFFF, 0xFF000000, 0, 0);
}
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
    while (1)
    {
        drawTime();
        wait();
        if ((c = readChar()))
        {
            if (c == '\n' && strcmp(commandBuffer, "exit"))
            {
                blank();
                free(commandBuffer);
                free(buffer);
                freePrints();
                return 1;
            }
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
                    addStringToBuffer(passCommand(commandBuffer), 1); // Nota: las instrucciones del TPE especifican que la shell debe moverse hacia arriba si se excede su espacio para texto.
                    freePrints();                                     // No se especifica el comportamiento esperado si el retorno de un 'comando' es mayor al espacio de la shell.
                    commandIndex = 0;                                 // El comportamiento default es simplemente no imprimir el retorno completo. Esto es lo que ocurre si a passCommand se le da parámetro 'ask' 0.
                } // De lo contrario, se imprimirá moviendo hacia arriba de a MOVE_BY líneas, requiriendo input del usuario.
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
    }
}

void addCharToBuffer(char c)
{
    buffer[index++] = c;
    buffer[index] = 0;
    paintCharOrWarp(c);
}
void addStringToBuffer(char *s, char ask)
{
    //  paintString(s, 0xffff0000, 0xff00ff00);
    char *aux = s;
    while (*aux)
        buffer[index++] = *(aux++);
    buffer[index] = 0;
    paintStringOrWarp(s, ask);
}
void paintStringOrWarp(char *s, char ask)
{
    if (strcmp(s, ""))
        return;
    if (!willFit(buffer))
    {
        if (ask) // informs user output does not fit, waits for input, warps output, if it still does not fit, continues informing and waiting for input and warping
        {
            char *prompt = "The output of this command will not fit. Press any key to continue output.";
            do
            {
                blank();
                paintString(buffer, letterColor, highlightColor);
                drawStringAt(prompt, 0xFF000000, 0xFFFFFFFF, 0, 0);
                getChar();
                warpNLines(MOVE_BY);
            } while (!willFit(buffer));
            blank();
            paintString(buffer, letterColor, highlightColor);
            drawTime();
        }
        else // simply warps the output
        {
            warpOneLine();
            while (!willFit(buffer))
                warpOneLine();
            blank();
            paintString(buffer, letterColor, highlightColor);
        }
    }
    else
        paintString(s, letterColor, highlightColor);
}
void paintCharOrWarp(char c)
{
    if (!paintChar(c, letterColor, highlightColor))
        warpAndRedraw();
}

void readUntilClose(int fd)
{
    paintStringOrWarp("\n", 0);
    char buffer[BLOCK];
    int n;
    while (n = read_sys(fd, buffer, BLOCK - 1))
    {
        for (int i = 0; i < n; i++)
        {
            if (!buffer[i])
            {
                paintStringOrWarp("\n", 0);
                paintStringOrWarp((char *)lineStart, 0);
                return; // TODO Remove this loop when read actually works
            }
        }
        buffer[n] = 0;
        paintStringOrWarp(buffer, 0);
    }
    paintStringOrWarp("\n", 0);
    paintStringOrWarp((char *)lineStart, 0);
}

char *passCommand(char *toPass)
{
    char mustRedraw = 0;
    stringOrFd pair = handleCommand(toPass, &mustRedraw);
    if (pair.s == NULL)
    {
        if (pair.fd > 0)
            readUntilClose(pair.fd);
        paintStringOrWarp(pair.fd + '0', 0);
        return "";
    }
    char *toPaint = pair.s;

    if (mustRedraw)
    {
        char *toReturn;
        if (strcmp(toPaint, ""))
        {
            if (strcmp(buffer, ""))
                toReturn = (char *)lineStart;
            else
                toReturn = sPrintf("%s\n%s", buffer, lineStart);
        }
        else
            toReturn = sPrintf("%s\n%s\n%s", buffer, toPaint, lineStart);
        index = 0;
        buffer[index] = 0;
        if (willFit(toReturn)) // so as not to blank when it won't fit, as the painting functions will blank anyways
            blank();
        return toReturn;
    }
    if (strcmp(toPaint, ""))
    {
        if (strcmp(buffer, ""))
            return (char *)lineStart;
        else
            return sPrintf("\n%s", lineStart);
    }
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
        if (!buffer[k - 1])
        {
            index = 0;
            buffer[index] = 0;
            return;
        }
    }
    for (j = k; buffer[j]; j++)
        buffer[j - k] = buffer[j];
    index = j - k;
    buffer[index] = 0;
}
void paintLineStart()
{
    paintString(lineStart, letterColor, highlightColor);
    const char *aux = lineStart;
    while (*aux)
    {
        buffer[index++] = *(aux++);
    }
    buffer[index] = 0;
}
