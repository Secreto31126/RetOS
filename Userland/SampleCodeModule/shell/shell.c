#include <stdint.h>
#include "shell.h"
#include "./../nstdlib/nstdlib.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"

char shellStart()
{
    char c;
    initialize();
    uint32_t width = 1024; // getWidth();
    uint32_t height = 768; // getHeight();
    char buffer[(width * height) / TRUE_LETTER_HEIGHT / TRUE_LETTER_WIDTH];
    uint64_t index = 0;
    HexColor *pixels = malloc((width * height * sizeof(HexColor)));
    pixels = 0x18000000;
    Window protoShell = getWindow(width, height, pixels);
    while ((c = getChar()) != 'q')
    {
        buffer[index++] = c;
        buffer[index] = 0;
        drawStringToWindow(protoShell, buffer, 0xFFFF0000, 0xFF00FF00, 1.0);
        drawWindow(protoShell, 0, 0);
    }
    return 1;
}