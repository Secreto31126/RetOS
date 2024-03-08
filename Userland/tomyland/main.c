/* main.c */

#include <sys.h>
#include <lib.h>

extern char bss;
extern char endOfBinary;

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
