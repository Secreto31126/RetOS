#include <console.h>
#include <lib.h>

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

#define WIDTH 80
#define HEADER_COLOR 0x2F
#define BODY_COLOR 0x1F
#define TEXT_COLOR 0x0F

#define PORT_COM1 0x3F8

static char buffer[64] = {'0'};
static const uint32_t width = WIDTH;
static const uint32_t height = 24;
static uint8_t *const header = (uint8_t *)0xB8000;
static uint8_t *const video = (uint8_t *)0xB8000 + WIDTH * 2; // 2 bytes per character
static uint8_t *currentVideo = (uint8_t *)0xB8000 + WIDTH * 2;

int is_transmit_empty()
{
	return input_byte(PORT_COM1 + 5) & 0x20;
}

void write_serial(char a)
{
	while (!is_transmit_empty())
		;
	output_byte(PORT_COM1, a);
}

void ncPrintHeader(const char *string)
{
	struct column
	{
		const char *string;
		int length;
		char dir;
	};

	uint8_t cols = 0;
	struct column columns[WIDTH] = {};

	for (int i = 0; string[i]; i++)
	{
		if (string[i] == '\b' || string[i] == '\n' || string[i] == '\e')
		{
			if (cols >= WIDTH)
			{
				break;
			}

			columns[cols].string = string + i + 1;
			columns[cols].length = 0;
			columns[cols].dir = string[i];
			cols++;
		}
		else if (cols)
		{
			// If trash character, don't count it
			if (string[i] != '\t')
			{
				columns[cols - 1].length++;
			}
		}
	}

	// If the user didn't specify any column
	if (!cols)
		return;

	const int col_size = width / cols;

	uint8_t *current = header;
	for (int i = 0; i < cols; i++)
	{
		current = header + i * col_size * 2;

		int from = 0, to = 0;
		switch (columns[i].dir)
		{
		case '\b':
			from = 0;
			to = columns[i].length;
			break;

		case '\n':
			from = (col_size - columns[i].length) / 2;
			to = from + columns[i].length;
			break;

		case '\e':
			from = col_size - columns[i].length;
			to = col_size;
			break;
		}

		const char *str = columns[i].string;
		for (int j = 0; current < video && j < col_size; j++)
		{
			// Skip trash characters
			while (*str == '\t')
				str++;

			*(current++) = from <= j && j < to ? *(str++) : ' ';
			*(current++) = HEADER_COLOR;
		}
	}

	// In case width is not divisible by cols
	while (current < video)
	{
		*(current++) = ' ';
		*(current++) = HEADER_COLOR;
	}
}

void ncPrint(const char *string)
{
	for (int i = 0; string[i] != 0; i++)
	{
		ncPrintChar(string[i], TEXT_COLOR);
	}
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

	if (color == TEXT_COLOR)
		write_serial(character);

	*currentVideo = character;
	currentVideo++;
	*currentVideo = color;
	currentVideo++;
}

void ncNewline()
{
	write_serial('\n');

	ncPrintChar(' ', TEXT_COLOR);
	while ((uint64_t)(currentVideo - video) % (width * 2))
	{
		ncPrintChar(' ', BODY_COLOR);
	}
}

void ncDeleteChar()
{
	write_serial('\b');

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
	write_serial('\t');

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
