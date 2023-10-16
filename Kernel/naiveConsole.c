#include <naiveConsole.h>

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

#define WIDTH 80
#define HEADER_COLOR 0x2F
#define BODY_COLOR 0x1F
#define TEXT_COLOR 0x0F

static char buffer[64] = {'0'};
static uint8_t *const video = (uint8_t *)0xB8000;
static uint8_t *currentVideo = (uint8_t *)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;

void ncPrint(const char *string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i], 0x1F);
}

void ncPrintChar(char character, char color)
{
	if (!character)
		return;

	if (character == '\n')
	{
		ncNewline();
		return;
	}

	if (character == '\b')
	{
		ncDeleteChar();
		return;
	}

	if (character == '\t')
	{
		ncTab();
		return;
	}

	*currentVideo = character;
	currentVideo++;
	*currentVideo = color;
	currentVideo++;
}

void ncNewline()
{
	ncPrintChar(' ', TEXT_COLOR);
	while ((uint64_t)(currentVideo - video) % (width * 2))
	{
		ncPrintChar(' ', BODY_COLOR);
	}
}

void ncDeleteChar()
{
	if (currentVideo <= video + 1)
	{
		*currentVideo = ' ';
		currentVideo[1] = BODY_COLOR;
		return;
	}

	currentVideo--;
	while (currentVideo > video + 1 && *currentVideo != TEXT_COLOR)
	{
		*currentVideo = BODY_COLOR;
		currentVideo -= 2;
	}

	*currentVideo = BODY_COLOR;
	currentVideo--;
	*currentVideo = ' ';
}

void ncTab()
{
	ncPrintChar(' ', TEXT_COLOR);
	while ((uint64_t)(currentVideo - video) % (4 * 2))
	{
		ncPrintChar(' ', BODY_COLOR);
	}
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
	uintToBase(value, buffer, base);
	ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
	{
		video[i * 2] = ' ';
		video[i * 2 + 1] = BODY_COLOR;
	}
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	// Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	// Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
