#include <stdint.h>
#include "shell.h"
#include "./../nstdlib/nstdlib.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"
#include "./../window/painter.h"
#include "./../snake/random.h"

char shellStart()
{
    char c;
    initialize();
    uint32_t width = getScreenWidth();
    uint32_t height = getScreenHeight();
    char buffer[(width * height) / TRUE_LETTER_HEIGHT / TRUE_LETTER_WIDTH];
    uint64_t index = 0;
    // HexColor *pixels = malloc((width * height * sizeof(HexColor)));
    // Window protoShell = getWindow(width, height, pixels);
    startPainter(width, height);
    setSize(1.0);
    static char lineStart[3] = ":~";
    setLineStart(lineStart);
    paintString("You are now in shell:\n", 0xFFFFFFFF, 0xFFFF00FF);
    char leaving = 0;
    while ((c = getChar()) != 'q' || !leaving)
    {
        if (c != 'q')
            setSeed(c);
        if (c == 'q')
            leaving = 1;
        else
            leaving = 0;
        if (c == '\b')
        {
            if (strcmp(buffer + index - strlen(lineStart) + 1, lineStart))
                continue;
            else
                index--;
        }
        else
            buffer[index++] = c;
        buffer[index] = 0;
        paintChar(c, 0xFFFFFFFF, 0xFF000000);
        // drawStringToWindow(protoShell, buffer, 0xFFFFFFFF, 0xFF101010, 1.0);
        // drawWindow(protoShell, 0, 0);
    }
    // free(pixels);
    endPainter();
    return 1;
}
