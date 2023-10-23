/* sampleCodeModule.c */

// char *v = (char *)0xB8000 + 79 * 2;
#include "nstdlib/nstdlib.h"
#include "window/window.h"
#include "window/fontInterface.h"
int main()
{
	/*
	printf("\nThis %s a %s long %s. The number here is -4*2:%d and here is 10*10*10:%u and this one is 456:%l. You will see some percentages here: % % %o %% \n", "is", "kinda", "string", -4 * 2, 10 * 10 * 10, 456);
	printf("\nYou will see a five here if this works: %d", pow(2.5, 2));
	for (int i = 0; i < 10; i++)
		printf("%c", getChar());
	*/
	char c;
	initialize();
	uint64_t width = getScreenHeight();
	uint64_t height = getScreenWidth();
	char buffer[(width * height) / TRUE_LETTER_HEIGHT / TRUE_LETTER_WIDTH];
	uint64_t index = 0;
	HexColor pixels[width * height];
	Window protoShell = getWindow(width, height, pixels);
	while ((c = getChar()))
	{
		buffer[index++] = c;
		buffer[index] = 0;
		drawStringToWindow(protoShell, buffer, 0xFFFFFFFF, 0x00000000, 1);
		drawWindow(protoShell, width, height);
	}
	return 0xDEADC0DE;
	return 0xDEADBEEF;
}
