/* main.c */

#include <sys.h>
#include <string.h>

extern char bss;
extern char endOfBinary;

#define puts(str) write(1, (str), strlen(str))

int main(int argc, char *argv[])
{
	// char pid = get_pid() + '0';
	// for (int i = 0; i < argc; i++)
	// {
	// 	write(1, &pid, 1);
	// 	puts(": ");
	// 	puts(argv[i]);
	// 	puts("\n");
	// }

	// int pid = fork();
	// if (pid)
	// {
	// 	while (1)
	// 	{
	// 		waitpid(-1, NULL, 0);
	// 	}
	// }

	int child_pid = fork();
	if (child_pid)
	{
		if (child_pid < 0)
		{
			puts("Error forking\n");
			return 1;
		}

		puts("My kid is alive\n");
		while (1)
		{
			int status = 0;
			char p = waitpid(-1, NULL, 0) + '1';
			char s = status + '0';

			puts("(");
			write(1, &p, 1);
			puts(" - 1) died with status ");
			write(1, &s, 1);
		}

		return 1;
	}

	int grandchild_pid = fork();
	if (grandchild_pid)
	{
		if (grandchild_pid < 0)
		{
			puts("Error forking\n");
			return 1;
		}

		puts("Hello, I'm the child, and I die inmediatelly\n");
		return 0;
	}

	puts("Hello, I'm the child's child\n");
	sleep(2);
	puts("I slept 2 seconds today 💀\n");
	return 0;
}
