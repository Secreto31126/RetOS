#include <stdint.h>
#include "shell.h"
#include "./../nstdlib/nstdlib.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"
#include "./../window/painter.h"
#include "./../snake/random.h"
#include "./../snake/snake.h"

#define MAX_COMMAND_LENGTH 30

char shellStart()
{
    uint32_t width = getScreenWidth();
    uint32_t height = getScreenHeight();
    uint64_t index = 0;
    uint64_t commandIndex = 0;

    char c;
    char commandBuffer[MAX_COMMAND_LENGTH + 1] = {0};
    char buffer[(width * height) / TRUE_LETTER_HEIGHT / TRUE_LETTER_WIDTH];
    // HexColor *pixels = malloc((width * height * sizeof(HexColor)));
    // Window protoShell = getWindow(width, height, pixels);
    initialize();
    startPainter(width, height);
    setSize(1.0);
    setLineStart(":~");
    paintString("You are now in shell:\n", -1, 0);
    char leaving = 0;
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
                commandBuffer[commandIndex++] = c;
            else
            {
                commandIndex = 0;
                if (strcmp(commandBuffer, "snake"))
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
        paintChar(c, 0xFFFFFFFF, 0xFF000000);
    }
    blank();
    endPainter();
    return 1;
}
