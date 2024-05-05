/* main.c */

#include <sys.h>
#include <lib.h>

extern char bss;
extern char endOfBinary;

int main(int argc, char *argv[])
{
	char pid = get_pid() + '0';
	for (int i = 0; i < argc; i++)
	{
		write(1, &pid, 1);
		write(1, ": ", 2);
		write(1, argv[i], strlen(argv[i]));
		write(1, "\n", 1);
	}

	beep(1000);

	if (fork())
	{
		char *args[] = {"Bye", NULL};
		return execv("module", args);
	}
	else
	{
		while (1)
		{
			halt_user();
		}
	}
}
