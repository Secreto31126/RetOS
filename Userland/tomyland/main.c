/* main.c */

#include <sys.h>
#include <string.h>

extern char bss;
extern char endOfBinary;

#define puts(str) write(1, (str), strlen(str))
static char flag = 0;

int main(int argc, char *argv[])
{
	// int pipefd[2];
	// int err = pipe(pipefd);

	// if (err)
	// {
	// 	beep(440);
	// 	sleep(2);
	// 	return 1;
	// }

	// err = dup2(pipefd[1], 1);

	// if (err < 0)
	// {
	// 	beep(440);
	// 	sleep(2);
	// 	return 1;
	// }

	// puts("Hello");

	// char str[10];
	// read(pipefd[0], str, 10);

	// write(2, str, strlen(str));

	// execv("module", NULL);
	// sleep(100);
	// return 1;
	if (flag)
	{
		char str[50] = {0};
		int n;
		while ((n = read(0, str, 50)) > 0)
		{
			write(1, str, n);
		}
		write(1, 0, 1);
		exit(0);
	}
	else
	{
		int pid = fork();
		if (pid)
		{
			while (1)
			{
				if (waitpid(-1, NULL, 0) < 0)
				{
					return 0;
				}
			}
		}

		flag = 1;
		execv("module", NULL);
		return 1;
	}
	return 1;

	// char char_pid = get_pid() + '0';
	// for (int i = 0; i < argc; i++)
	// {
	// 	write(1, &char_pid, 1);
	// 	puts(": ");
	// 	puts(argv[i]);
	// 	puts("\n");
	// }

	// int child_pid = fork();
	// if (child_pid)
	// {
	// 	if (child_pid < 0)
	// 	{
	// 		puts("Error forking\n");
	// 		return 1;
	// 	}

	// 	// puts("My kid is alive\n");
	// 	while (1)
	// 	{
	// 		int status = 0;
	// 		int pid = waitpid(-1, &status, 0);

	// 		if (pid < 0)
	// 		{
	// 			puts("I ran out of kids to await...\n");
	// 			break;
	// 		}

	// 		char p = pid + '0';
	// 		char s = status + '0';

	// 		puts("PID ");
	// 		write(1, &p, 1);
	// 		puts(" died with status ");
	// 		write(1, &s, 1);
	// 		puts("\n");
	// 	}

	// 	return 1;
	// }

	// int grandchild_pid = fork();
	// if (grandchild_pid)
	// {
	// 	if (grandchild_pid < 0)
	// 	{
	// 		puts("Error forking\n");
	// 		return 1;
	// 	}

	// 	puts("Hello, I'm the child, and I die inmediatelly\n");
	// 	return 0;
	// }

	// puts("Hello, I'm the child's child\n");
	// sleep(1);
	// puts("I slept 1 second today 💀\n");
	// return 1;
}
