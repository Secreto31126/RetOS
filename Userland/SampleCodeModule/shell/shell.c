#include <stdint.h>
#include "shell.h"
#include "./../nstdlib/nstdlib.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"

char shellStart()
{

    HexColor testPixels[900];
    for (int i = 0; i < 900; i++)
        testPixels[i] = 0xFFFFFFFF;
    Window testW = getWindow(30, 30, testPixels);
    drawWindow(testW, 10, 10);

    char c;
    initialize();
    uint32_t width = 1024;
    uint32_t height = 768;
    printf("w:%dh:%d", width, height);
    char buffer[(width * height) / TRUE_LETTER_HEIGHT / TRUE_LETTER_WIDTH];
    uint64_t index = 0;
    HexColor pixels = malloc((width * height * sizeof(HexColor)));
    Window protoShell = getWindow(width, height, pixels);
    while ((c = getChar()) != 'q')
    {
        printf("%c", c);
        buffer[index++] = c;
        buffer[index] = 0;
        printf("In1");
        drawStringToWindow(protoShell, buffer, 0x87654321, 0x12345678, 1.0);
        printf("In2");
        drawWindow(protoShell, 0, 0);
        printf("In3");
    }
    return 1;
}