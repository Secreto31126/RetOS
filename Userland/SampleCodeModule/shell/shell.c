#include <stdint.h>
#include "shell.h"
#include "./../nstdlib/nstdlib.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"
#include "./../window/painter.h"
#include "./../snake/random.h"
#include "./../snake/snake.h"

#define MAX_COMMAND_LENGTH 90 // Absolutely arbitrary
void warpLineUp(int lines);

static char *buffer, *commandBuffer;
static uint64_t index, commandIndex;

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
    paintString("You are now in shell:\n", -1, 0);
    char leaving = 0;
    setSize(1.0);
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
                    paintString("You are now in shell. Player ", -1, 0);
                    paintChar(winner + '0', -1, 0);
                    paintString(" won", -1, 0);
                }
            }
        }
        commandBuffer[commandIndex] = 0;
        buffer[index] = 0;
        if (!paintChar(c, 0xFFFFFFFF, 0xFF000000))
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
    paintString(buffer, -1, 0);
}