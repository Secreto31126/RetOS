#include "shell.h"
#include "commandHandler/commandHandler.h"
#define BLOCK 50
#define MOVE_BY 8
#define SHELL_INTRO "You are now in shell:\n"
#define LINE_START ":~ "
void warpNLines(shell s, uint64_t n);
void warpAndRedraw();
void warpOneLine();
char *passCommand(shell s, char *toPass);
void paintLineStart();
void paintCharOrWarp(shell s, char c);
void paintStringOrWarp(shell s, char *str, char ask);
void addStringToBuffer(shell s, char *str, char ask);
void addCharToBuffer(shell s, char c);

void drawTime(painter p)
{
    drawStringAt(p, getTimeString(), 0xFFFFFFFF, 0xFF000000, 0, 0);
}
char shellStart(painter p)
{
    uint32_t width = getScreenWidth();
    uint32_t height = getScreenHeight();
    shell s = malloc(sizeof(shellData));

    s->p = p;
    s->letterColor = 0xFF000000 | HEX_WHITE;
    s->highlightColor = 0xFF000000 | HEX_BLACK;
    s->fromLastEnter = 0;
    s->index = 0;
    s->commandIndex = 0;
    s->commandBuffer = malloc(sizeof(char) * (MAX_COMMAND_LENGTH + 1));
    s->buffer = malloc((width * height) / TRUE_LETTER_HEIGHT / TRUE_LETTER_WIDTH);
    *(s->buffer) = 0;
    *(s->commandBuffer) = 0;
    s->index += sPuts(s->buffer, SHELL_INTRO);

    paintString(p, s->buffer, s->letterColor, s->highlightColor);
    paintLineStart(p);
    char c;
    while (1)
    {
        drawTime(p);
        wait();
        if ((c = readChar()))
        {
            if (c == '\n' && strcmp(s->commandBuffer, "exit"))
            {
                blank(p);
                free(s->commandBuffer);
                free(s->buffer);
                freePrints();
                free(s);
                return 1;
            }
            if (c == '\b')
            {
                if (!s->fromLastEnter || !s->index)
                {
                    continue;
                }
                else
                {
                    paintChar(s->p, c, s->letterColor, s->highlightColor);
                    s->index--;
                    s->commandIndex--;
                    s->fromLastEnter--;
                }
            }
            else
            {
                if (c == '\n')
                {
                    s->fromLastEnter = 0;
                    addStringToBuffer(s, passCommand(s, s->commandBuffer), 1); // Nota: las instrucciones del TPE especifican que la shell debe moverse hacia arriba si se excede su espacio para texto.
                    freePrints();                                              // No se especifica el comportamiento esperado si el retorno de un 'comando' es mayor al espacio de la shell.
                    s->commandIndex = 0;                                       // El comportamiento default es simplemente no imprimir el retorno completo. Esto es lo que ocurre si a passCommand se le da parámetro 'ask' 0.
                } // De lo contrario, se imprimirá moviendo hacia arriba de a MOVE_BY líneas, requiriendo input del usuario.
                else
                {
                    addCharToBuffer(s, c);
                    s->fromLastEnter++;
                    if (s->commandIndex < MAX_COMMAND_LENGTH)
                        s->commandBuffer[s->commandIndex++] = c; // ignores commands after a certain length (since no commands should be that long)
                }
            }
            s->commandBuffer[s->commandIndex] = 0;
            s->buffer[s->index] = 0;
        }
    }
}

void addCharToBuffer(shell s, char c)
{
    s->buffer[s->index++] = c;
    s->buffer[s->index] = 0;
    paintCharOrWarp(s, c);
}
void addStringToBuffer(shell s, char *str, char ask)
{
    //  paintString(s, 0xffff0000, 0xff00ff00);
    char *aux = str;
    while (*aux)
        s->buffer[s->index++] = *(aux++);
    s->buffer[s->index] = 0;
    paintStringOrWarp(s, str, ask);
}
void paintStringOrWarp(shell s, char *str, char ask)
{
    if (strcmp(str, ""))
        return;
    if (!willFit(s->p, s->buffer))
    {
        if (ask) // informs user output does not fit, waits for input, warps output, if it still does not fit, continues informing and waiting for input and warping
        {
            char *prompt = "The output of this command will not fit. Press any key to continue output.";
            do
            {
                blank(s->p);
                paintString(s->p, s->buffer, s->letterColor, s->highlightColor);
                drawStringAt(s->p, prompt, 0xFF000000, 0xFFFFFFFF, 0, 0);
                getChar();
                warpNLines(s->p, MOVE_BY);
            } while (!willFit(s->p, s->buffer));
            blank(s->p);
            paintString(s->p, s->buffer, s->letterColor, s->highlightColor);
            drawTime(s->p);
        }
        else // simply warps the output
        {
            warpOneLine(s->p);
            while (!willFit(s->p, s->buffer))
                warpOneLine(s->p);
            blank(s->p);
            paintString(s->p, s->buffer, s->letterColor, s->highlightColor);
        }
    }
    else
        paintString(s->p, str, s->letterColor, s->highlightColor);
}
void paintCharOrWarp(shell s, char c)
{
    if (!paintChar(s->p, c, s->letterColor, s->highlightColor))
        warpAndRedraw(s->p);
}

char *passCommand(shell s, char *toPass)
{
    char mustRedraw = 0;
    char *toPaint = handleCommand(toPass, &mustRedraw);

    if (mustRedraw)
    {
        char *toReturn;
        if (strcmp(toPaint, ""))
        {
            if (strcmp(s->buffer, ""))
                toReturn = (char *)(s->lineStart);
            else
                toReturn = sPrintf("%s\n%s", s->buffer, s->lineStart);
        }
        else
            toReturn = sPrintf("%s\n%s\n%s", s->buffer, toPaint, s->lineStart);
        s->index = 0;
        s->buffer[s->index] = 0;
        if (willFit(s->p, toReturn)) // so as not to blank when it won't fit, as the painting functions will blank anyways
            blank(s->p);
        return toReturn;
    }
    if (strcmp(toPaint, ""))
    {
        if (strcmp(s->buffer, ""))
            return (char *)(s->lineStart);
        else
            return sPrintf("\n%s", s->lineStart);
    }
    return sPrintf("\n%s\n%s", toPaint, s->lineStart);
}
void warpOneLine(shell s)
{
    warpNLines(s, 1);
}
void warpAndRedraw(shell s)
{
    warpOneLine(s);
    blank(s->p);
    paintString(s->p, s->buffer, s->letterColor, s->highlightColor);
}
void setLetterColor(shell s, HexColor color) // command handler is responsible for setting mustRedraw to 1
{
    s->letterColor = 0xFF000000 | color; // Letters cannot be transparent
}
void setHighlightColor(shell s, HexColor color) // command handler is responsible for setting mustRedraw to 1
{
    s->highlightColor = color; // highlights can be transparent
}
void resize(shell s, double size) // command handler is responsible for setting mustRedraw to 1
{
    setSize(s->p, size);
}

void clearShell(shell s)
{
    blank(s->p);
    s->index = 0;
    s->buffer[s->index] = 0;
    s->commandIndex = 0;
    s->commandBuffer[s->commandIndex] = 0;
    s->fromLastEnter = 0;
}

void warpNLines(shell s, uint64_t n) // char or char* you want to add must be in buffer already. This shortens the buffer from the start so that it fits, then repaints it.
{
    if (!n)
        return;
    uint64_t max = getCharPerLine(s->p);
    int i = 0, j, k = 0;
    while (n)
    {
        for (i = 0; i < max && s->buffer[k + i] && s->buffer[k + i] != '\n'; i++)
            ;
        n--;
        i++;
        k += i;
        if (!((s->buffer)[k - 1]))
        {
            s->index = 0;
            s->buffer[s->index] = 0;
            return;
        }
    }
    for (j = k; s->buffer[j]; j++)
        s->buffer[j - k] = s->buffer[j];
    s->index = j - k;
    s->buffer[s->index] = 0;
}
void paintLineStart(shell s)
{
    paintString(s->p, LINE_START, s->letterColor, s->highlightColor);
    const char *aux = LINE_START;
    while (*aux)
    {
        s->buffer[s->index++] = *(aux++);
    }
    s->buffer[s->index] = 0;
}
