/* main.c */

#include <sys.h>
#include <stdint.h>

extern char bss;
extern char endOfBinary;

int main(char *);
void *memset(void *destiny, int32_t c, uint64_t length);

int main(char *name)
{
	write(1, "Welcome to ", 12);
	write(1, name, 9);
	write(1, "!\n", 3);

	return 0xDEADBEEF;

	char c;
	beep(1000);
	while (!read(0, &c, 1))
		halt_user();
	beep(0);

	return 0xDEADC0DE;
}

int c_start(char *name)
{
	// Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);
	return main(name);
}

void *memset(void *destiation, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char *dst = (char *)destiation;

	while (length--)
		dst[length] = chr;

	return destiation;
}
