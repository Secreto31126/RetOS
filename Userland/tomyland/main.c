/* main.c */

#include <sys.h>
#include <stdint.h>
#include <stddef.h>

extern char bss;
extern char endOfBinary;

int main(int argc, char *argv[]);
void *memset(void *destiny, int32_t c, uint64_t length);
size_t strlen(const char *str);

int main(int argc, char *argv[])
{
	for (int i = 0; i < argc; i++)
	{
		write(1, argv[i], strlen(argv[i]));
		write(1, "\n", 1);
	}

	char c;
	beep(1000);
	while (!read(0, &c, 1))
		halt_user();
	beep(0);

	return 0xDEADC0DE;
}

int c_start(char **argv)
{
	// Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	int argc = 0;
	while (argv[argc])
		argc++;

	return main(argc, argv);
}

void *memset(void *destiation, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char *dst = (char *)destiation;

	while (length--)
		dst[length] = chr;

	return destiation;
}

size_t strlen(const char *str)
{
	const char *s = str;
	while (*s)
		s++;
	return s - str;
}
