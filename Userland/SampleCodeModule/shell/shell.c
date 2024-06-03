#include "shell.h"
#include "commandHandler/commandHandler.h"
#include <stdint.h>
#include <string.h>
#define BLOCK 50
#define READ_BLOCK 500
#define MOVE_BY 8
void warpNLines(uint64_t n);
void warpAndRedraw();
void warpOneLine();
char *passCommand(char *toPass);
void paintLineStart();
void paintCharOrWarp(char c);
void paintStringOrWarp(const char *s, char ask);
void addStringToBuffer(const char *s, char ask);
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
        if ((c = readChar()))
        {
            if (c == '\n' && !strcmp(commandBuffer, "exit"))
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
void addStringToBuffer(const char *s, char ask)
{
    const char *aux = s;
    while (*aux)
        buffer[index++] = *(aux++);
    buffer[index] = 0;
    paintStringOrWarp(s, ask);
}
void paintStringOrWarp(const char *s, char ask)
{
    if (!strcmp(s, ""))
        return;
    char flag = 0;
    if (ask)
    {
        char *prompt = "The output of this command will not fit. Press any key to continue output.";
        while (!willFit(buffer))
        {
            flag = 1;
            blank();
            paintString(buffer, letterColor, highlightColor);
            drawStringAt(prompt, 0xFF000000, 0xFFFFFFFF, 0, 0);
            getChar();
            warpNLines(MOVE_BY);
        }
        if (flag)
        {
            blank();
            paintString(buffer, letterColor, highlightColor);
            drawTime();
        }
        else
        {
            paintString(s, letterColor, highlightColor);
        }
    }
    else
    {
        while (!willFit(buffer))
        {
            flag = 1;
            warpOneLine();
        }
        if (flag)
        {
            blank();
            paintString(buffer, letterColor, highlightColor);
            drawTime();
        }
        else
        {
            paintString(s, letterColor, highlightColor);
        }
    }
}
void paintCharOrWarp(char c)
{
    if (!paintChar(c, letterColor, highlightColor))
        warpAndRedraw();
}

int handleStdKeys(moduleData data, displayStyles displayStyle)
{
    char r_buffer[BLOCK];
    int n = read_sys(STD_KEYS, r_buffer, BLOCK - 1);
    if (n < 0)
        return 1;
    r_buffer[n] = 0;

    char ctrlC[] = {LCTRL, 0x2E, 0};
    char ctrlD[] = {LCTRL, 0x20, 0};
    if (strstr(r_buffer, ctrlC) != NULL)
    {
        return 2;
    }
    if (strstr(r_buffer, ctrlD) != NULL && data.writeFd >= 0)
    {
        // This is equivalent to sending EOF through the pipe
        close(data.writeFd);
    }
    return 0;
}

int handleReadFd(moduleData data, displayStyles displayStyle)
{
    char r_buffer[BLOCK];
    int n = read_sys(data.fd, r_buffer, BLOCK - 1);
    if (n < 0)
        return 1;
    if (n == 0)
        return 2;
    r_buffer[n] = 0;
    addStringToBuffer(r_buffer, 0);
    return 0;
}

int handleWriteFd(moduleData data, displayStyles displayStyle)
{
    char r_buffer[BLOCK];

    int n = read_sys(STD_IN, r_buffer, BLOCK - 1);
    if (n < 0)
        return 1;

    if (displayStyle != REDRAW_ALWAYS)
    {
        r_buffer[n] = 0;
        addStringToBuffer(r_buffer, 0);
    }

    if (data.writeFd >= 0 && print_sys(data.writeFd, r_buffer, n) < 0)
        return 1;
    return 0;
}

void killFgAndLeave(moduleData data, char *message)
{
    close(data.fd);
    if (data.writeFd >= 0)
        close(data.writeFd);
    if (data.cPid >= 0)
    {
        kill(data.cPid, SIGKILL);
        waitpid(data.cPid, NULL, 0);
    }
    addStringToBuffer(message, 0);
    addStringToBuffer("\n", 0);
    addStringToBuffer((char *)lineStart, 0);
}

void readUntilClose(moduleData data, displayStyles displayStyle)
{
    flush(STD_KEYS);
    int readFds[3] = {data.fd, STD_IN, STD_KEYS}, availableReadFds[3] = {0};
    int readFdCount = 3, availableReadFdCount = 0;

    addStringToBuffer("\n", 0);

    // this loop could be simplified with a very ugly and hard to read function
    char leaveFlag = 0;
    while (!leaveFlag)
    {
        availableReadFdCount = pselect(readFdCount, readFds, availableReadFds);
        for (int i = 0; i < availableReadFdCount; i++)
        {
            if (data.fd == availableReadFds[i])
            {
                char aux = handleReadFd(data, displayStyle);
                if (aux == 1)
                {
                    killFgAndLeave(data, "Communication failed, killed foreground process.\n");
                    return;
                }
                if (aux == 2)
                { // process stopped writing, wait for it to terminate normally
                    leaveFlag = 1;
                    break;
                }
            }
            else if (STD_KEYS == availableReadFds[i])
            {
                char aux = handleStdKeys(data, displayStyle);
                if (aux == 1)
                {
                    killFgAndLeave(data, "Communication failed, killed foreground process.\n");
                    return;
                }
                if (aux == 2)
                { // send SIGKILL to process, wait for it to terminate
                    killFgAndLeave(data, "");
                    return;
                }
            }
            else if (STD_IN == availableReadFds[i])
            {
                if (handleWriteFd(data, displayStyle))
                {
                    killFgAndLeave(data, "Communication failed, killed foreground process.\n");
                    return;
                }
            }
        }
    }

    close(data.fd);
    if (data.writeFd >= 0)
    {
        close(data.writeFd);
    }
    waitpid(data.cPid, NULL, 0);

    addStringToBuffer("\n", 0);
    addStringToBuffer((char *)lineStart, 0);
}

char *passCommand(char *toPass)
{
    displayStyles displayStyle = 0;
    moduleData data = handleCommand(toPass, &displayStyle);

    if (data.s == NULL)
    {
        if (data.fd >= 0)
        {
            readUntilClose(data, displayStyle);
        }
        else
        {
            addStringToBuffer("\n", 0);
            addStringToBuffer(lineStart, 0);
        }
        return "";
    }

    char *toPaint = data.s;

    if (displayStyle >= REDRAW_ONCE)
    {
        char *toReturn;
        if (!strcmp(toPaint, ""))
        {
            if (!strcmp(buffer, ""))
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
    else if (!strcmp(toPaint, ""))
    {
        if (!strcmp(buffer, ""))
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
void setLetterColor(HexColor color) // command handler is responsible for setting displayStyle to REDRAW_ONCE
{
    letterColor = 0xFF000000 | color; // Letters cannot be transparent
}
void setHighlightColor(HexColor color) // command handler is responsible for setting displayStyle to REDRAW_ONCE
{
    highlightColor = color; // highlights can be transparent
}
void resize(double size) // command handler is responsible for setting displayStyle to REDRAW_ONCE
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
