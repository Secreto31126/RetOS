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

	int child_pid = fork();
	int child = child_pid == 0;

	if (child)
	{
		if (fork())
		{
			puts("Hello, I'm the child, and I die inmediatelly\n");
			return 0;
		}

		puts("Hello, I'm the child's child\n");
		sleep(5);
		puts("I slept 5 seconds today 💀\n");
		return 0;
	}
	else
	{
		if (child_pid < 0)
		{
			puts("Error forking\n");
			return 1;
		}

		puts("My kid is alive\n");
		waitpid();
		puts("My kid died happily...\n");

		return 0;
	}
}
