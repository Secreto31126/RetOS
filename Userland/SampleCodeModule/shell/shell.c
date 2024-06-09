#include "shell.h"
#include "commandHandler/commandHandler.h"
#include <stdint.h>
#include <string.h>

#define BLOCK 50
#define READ_BLOCK 500
#define MOVE_BY 8
#define MAX_ACTIVE 256

void warpNLines(uint64_t n);
void warpAndRedraw();
void warpOneLine();
char *passCommand(char *toPass);
void paintLineStart();
void paintCharOrWarp(char c);
void paintStringOrWarp(const char *s, char ask);
void addStringToBuffer(const char *s, char ask);
void addCharToBuffer(char c);
int handleReadFd(moduleData data, displayStyles displayStyle);
void killModule(commandData cData, char *message);
void readUntilClose(commandData cData, displayStyles displayStyle);

// String constants
static const char *lineStart = ":~ ";
static const char *shellIntro = "You are now in shell:\n";

// Tracks colors for painter
static HexColor letterColor = 0xFF000000 | HEX_WHITE, highlightColor = 0xFF000000 | HEX_BLACK;

// For input management
static char *buffer, *commandBuffer;
static uint64_t index, commandIndex;
static char fromLastEnter = 0;

// For background module management
static commandData activeReads[MAX_ACTIVE] = {{{NULL, STD_IN, -1, -1}, NULL, 0}};
static int activeReadsCount = 1;

void drawTime()
{
    char buffer[6];
    drawStringAt(getTimeString(buffer), 0xFFFFFFFF, 0xFF000000, 0, 0);
}

char addToActive(commandData bgModule)
{
    if (activeReadsCount >= MAX_ACTIVE)
        return 1;
    activeReads[activeReadsCount++] = bgModule;
    return 0;
}

int getFdIndex(int fd)
{
    int index = 0;
    while (activeReads[index].data.fd != fd)
        index++;
    return index;
}

void removeFromActive(int index)
{
    // do not remove STD_IN, do not remove beyond array
    if (index >= activeReadsCount || !index)
        return;

    // just shift them all back by 1, starting at the index to remove
    activeReadsCount--;
    for (; index < activeReadsCount; index++)
        activeReads[index] = activeReads[index + 1];
}

int getPidIndex(int pid)
{
    for (int i = 1; i < activeReadsCount; i++)
    {
        int count = activeReads[i].cPidCount;
        int *cPids = activeReads[i].cPid;
        for (int j = 0; j < count; j++)
        {
            if (cPids[j] == pid)
                return i;
        }
    }
    return -1;
}

void bringToFg(int pid)
{
    int index;
    if (activeReadsCount <= 1)
        return;
    if (pid < 0)
        index = 1;
    else
        index = getPidIndex(pid);
    if (index < 1)
        return;
    commandData toBring = activeReads[index];
    removeFromActive(index);
    readUntilClose(toBring, APPEND);
}

char readAsInput(char c)
{
    if (c == '\n')
    {
        if (!strcmp(commandBuffer, "exit"))
            return 1;
        if (isPrefix("fg", commandBuffer))
            return 2;
    }
    if (c == '\b')
    {
        if (!fromLastEnter || !index)
        {
            return 0;
        }
        else
        {
            int charsToErase = buffer[index - 1] == '\t' ? SPACES_IN_TAB : 1;
            while (charsToErase > 0)
            {
                paintChar(c, letterColor, highlightColor);
                charsToErase--;
            }
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
    return 0;
}

// returns 1 if content was printed
int readAsBackground(int index, displayStyles displayStyle)
{
    commandData cData = activeReads[index];
    int aux = handleReadFd(cData.data, displayStyle);
    if (aux == 1)
    {
        killModule(cData, "Communication failed, killed background process.\n");
        removeFromActive(index);
        return 1;
    }
    if (aux == 2)
    {
        // in case the process did not end on its own, kill it, as it has no useful output to give
        killModule(cData, "");
        removeFromActive(index);
        return 0;
    }
    return 1;
}

char shellLoop()
{
    drawTime();

    int canBeRead[MAX_ACTIVE];
    int activeReadFds[MAX_ACTIVE];
    for (int i = 0; i < activeReadsCount; i++)
    {
        activeReadFds[i] = activeReads[i].data.fd;
    }
    int n = pselect(activeReadsCount, activeReadFds, canBeRead);
    for (int i = 0; i < n; i++)
    {
        if (canBeRead[i] == STD_IN)
        {
            char c;
            int aux = read_sys(STD_IN, &c, 1);
            if (aux <= 0)
            {
                blank();
                addStringToBuffer("There was an error reading STD_IN.\nExiting.", 0);
                sleep(1);
                exit(1);
                return 1;
            }
            aux = readAsInput(c);
            if (aux == 1) // exit
            {
                blank();
                free(commandBuffer);
                free(buffer);
                freePrints();
                return 1;
            }
            if (aux == 2) // bring to fg
            {
                bringToFg(atoi(shiftToNextWord(commandBuffer)));
                addCharToBuffer('\n');
                addStringToBuffer(lineStart, 0);
                commandIndex = 0;
                fromLastEnter = 0;
            }
            // Handle all user input before printing bg processes (makes terminal more reseponsive)
            return 0;
        }
        else
        {
            if (readAsBackground(getFdIndex(canBeRead[i]), AS_BACKGROUND))
            {
                addCharToBuffer('\n');
                addStringToBuffer(lineStart, 0);
                fromLastEnter = 0;
            }
        }
    }
    return 0;
}

char shellStart()
{
    uint32_t width = getScreenWidth();
    uint32_t height = getScreenHeight();

    index = 0;
    commandIndex = 0;
    commandBuffer = malloc(sizeof(char) * (MAX_COMMAND_LENGTH + 1));
    buffer = malloc((width * height) / TRUE_LETTER_HEIGHT / TRUE_LETTER_WIDTH);
    if (buffer == NULL || commandBuffer == NULL)
    {
        paintStringOrWarp("Could not initialize shell.\n", 0);
        sleep(1);
        return 0;
    }
    *buffer = 0;
    *commandBuffer = 0;
    index += sPuts(buffer, shellIntro);
    paintString(buffer, letterColor, highlightColor);
    paintLineStart();

    while (!shellLoop())
        ;
    return 0;
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
    char r_buffer[READ_BLOCK];
    int n = read_sys(data.fd, r_buffer, READ_BLOCK - 1);
    if (n < 0)
        return 1;
    if (n == 0)
        return 2;
    r_buffer[n] = 0;
    if (displayStyle == REDRAW_ALWAYS)
    {
        blank();
        paintStringOrWarp(r_buffer, 0);
    }
    else
    {
        if (displayStyle == AS_BACKGROUND)
            addCharToBuffer('\n');
        addStringToBuffer(r_buffer, 0);
    }
    return 0;
}

int handleWriteFd(moduleData data, displayStyles displayStyle)
{
    char r_buffer[BLOCK];

    int n = read_sys(STD_IN, r_buffer, BLOCK - 1);
    if (n < 0)
        return 1;

    if (displayStyle != REDRAW_ALWAYS && displayStyle != NO_STDIN)
    {
        r_buffer[n] = 0;
        addStringToBuffer(r_buffer, 0);
    }

    if (data.writeFd >= 0 && print_sys(data.writeFd, r_buffer, n) < 0)
        return 1;
    return 0;
}

void killModule(commandData cData, char *message)
{
    moduleData data = cData.data;
    close(data.fd);
    if (data.writeFd >= 0)
        close(data.writeFd);

    for (int i = 0; i < cData.cPidCount; i++)
        if (cData.cPid[i] >= 0)
        {
            kill(cData.cPid[i], SIGKILL);
            waitpid(cData.cPid[i], NULL, 0);
        }
    free(cData.cPid);
    addStringToBuffer(message, 0);

    // just in case
    shut();
}

void readUntilClose(commandData cData, displayStyles displayStyle)
{
    flush(STD_KEYS);
    moduleData data = cData.data;

    int fullReadFds[MAX_ACTIVE + 2] = {data.fd, STD_IN, STD_KEYS}, fullReadFdCount, fullAvailableReadFds[MAX_ACTIVE + 2] = {0}, fullAvailableReadFdCount;

    addStringToBuffer("\n", 0);

    // this loop could be simplified with a very ugly and hard to read function
    char leaveFlag = 0;
    while (!leaveFlag)
    {
        fullReadFdCount = 2 + activeReadsCount;
        // Do not start from activeReads[0], as STD_IN is already here. It is best if STD_IN input is processed first (looks better on terminal)
        for (int i = 3; i < fullReadFdCount; i++)
        {
            fullReadFds[i] = activeReads[i - 2].data.fd;
        }
        fullAvailableReadFdCount = pselect(fullReadFdCount, fullReadFds, fullAvailableReadFds);
        for (int i = 0; i < fullAvailableReadFdCount; i++)
        {
            int currentFd = fullAvailableReadFds[i];
            // it is foreground process
            if (data.fd == currentFd)
            {
                char aux = handleReadFd(data, displayStyle);
                if (aux == 1)
                {
                    killModule(cData, "Communication failed, killed foreground process.\n");
                    return;
                }
                if (aux == 2)
                { // process stopped writing, wait for it to terminate normally
                    leaveFlag = 1;
                    break;
                }
            }
            // a key has been pressed
            else if (STD_KEYS == currentFd)
            {
                char aux = handleStdKeys(data, displayStyle);
                if (aux == 1)
                {
                    killModule(cData, "Communication failed, killed foreground process.\n");
                    return;
                }
                if (aux == 2)
                { // send SIGKILL to process, wait for it to terminate
                    killModule(cData, "");
                    return;
                }
            }
            // a key that produces input has been pressed
            else if (STD_IN == currentFd)
            {
                if (handleWriteFd(data, displayStyle))
                {
                    killModule(cData, "Communication failed, killed foreground process.\n");
                    return;
                }
            }
            // it is background process
            else
            {
                readAsBackground(getFdIndex(currentFd), APPEND);
            }
        }
    }
    close(data.fd);
    if (data.writeFd >= 0)
    {
        close(data.writeFd);
    }
    if (data.cPid >= 0)
    {
        waitpid(data.cPid, NULL, 0);
    }
    free(cData.cPid);
}

char *passCommand(char *toPass)
{
    displayStyles displayStyle = 0;
    int cPidBuffer = malloc(sizeof(int) * MAX_PIPES);
    if (cPidBuffer == NULL)
    {
        addStringToBuffer("\nCould not allocate memory for command\n", 0);
        addStringToBuffer(lineStart, 0);
        return "";
    }
    commandData cData = handleCommand(toPass, &displayStyle, cPidBuffer);
    moduleData mData = cData.data;

    if (mData.s == NULL)
    {
        if (mData.fd >= 0)
        {
            if (displayStyle == AS_BACKGROUND)
            {
                addToActive(cData);
            }
            else
            {
                readUntilClose(cData, displayStyle);
                if (displayStyle == REDRAW_ALWAYS || displayStyle == REDRAW_ONCE)
                {
                    blank();
                    paintStringOrWarp(buffer, 0);
                }
            }
        }

        addStringToBuffer("\n", 0);
        addStringToBuffer(lineStart, 0);
        return "";
    }

    char *toPaint = mData.s;

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
