#include <stdint.h>
#include "shell.h"
#include "./../nstdlib/nstdlib.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"

char shellStart()
{
    /*
    for (char *i = 0x10000000; i < 0x18000000; i++)
        if (*i)
            printf("%c", *i);
    printf("Done first\n");
    for (char *i = 0x18000000; i < 0x20000000; i++)
        if (*i)
            printf("%d|", *i);
    printf("Done\n");
    */
    /*
        HexColor *testPixels = malloc(900 * sizeof(HexColor));
        // HexColor *testPixels = 0x10000000;
        printf("testPixels: %l", testPixels);
        for (int i = 0; i < 900; i++)
            testPixels[i] = 0xFFFFFFFF;
        for (int i = 0; i < 900; i++)
            printf("%l", testPixels[i]);
*/
    HexColor *testPixels = 0x18000000;
    for (int i = 0; i < 900; i++)
        testPixels[i] = 0xFFFFFFFF;
    Window testW = getWindow(30, 30, testPixels);
    drawWindow(testW, 0, 0);

    char c;
    initialize();
    uint32_t width = 1024; // getWidth();
    uint32_t height = 768; // getHeight();
    char buffer[(width * height) / TRUE_LETTER_HEIGHT / TRUE_LETTER_WIDTH];
    uint64_t index = 0;
    HexColor pixels = malloc((width * height * sizeof(HexColor)));
    pixels = 0x18000000;
    Window protoShell = getWindow(width, height, pixels);
    while ((c = getChar()) != 'q')
    {
        printf("%c", c);
        buffer[index++] = c;
        buffer[index] = 0;
        drawStringToWindow(protoShell, buffer, 0xFFFF0000, 0xFF00FF00, 1.0);
        drawWindow(protoShell, 0, 0);
    }
    return 1;
}