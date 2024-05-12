/* main.c */

#include <sys.h>
#include <string.h>

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

	int child_pid = fork();
	int child = child_pid == 0;
	int parent = child_pid != 0;

	if (parent)
	{
		char *args[] = {"Bye", NULL};
		return execv("module", args);
	}
	else
	{
		if (child_pid < 0)
		{
			write(1, "Error forking\n", 14);
			return 1;
		}

		return 0;
	}
}
