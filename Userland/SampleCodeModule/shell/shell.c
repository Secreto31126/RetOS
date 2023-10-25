#include <stdint.h>
#include "shell.h"
#include "./../nstdlib/nstdlib.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"
#include "./../window/painter.h"

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
    setLineStart(":~");
    while ((c = getChar()) != 'q')
    {
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