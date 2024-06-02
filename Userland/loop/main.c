/* main.c */

#include <sys.h>
#include <string.h>
#include <stdlib.h>

extern char bss;
extern char endOfBinary;

#define BLOCK 6
#define DEFAULT_INTERVAL 10

int main(int argc, char *argv[])
{
	int interval;
	if (argc > 0)
		interval = atoi(argv[0]);
	else
	{
		char aux[BLOCK];
		int n;
		if ((n = read(STD_IN, aux, BLOCK - 1)) > 0)
		{
			aux[n] = 0;
			interval = atoi(aux);
		}
		else
		{
			interval = DEFAULT_INTERVAL;
		}
	}
	if (interval <= 0)
		interval = 1;

	char pidString[BLOCK];
	itoa(get_pid(), pidString, 10);
	int len = strlen(pidString);

	while (1)
	{
		// we have an sprintf, but it uses malloc, is not in globals, and really is just a horrifying implementation. Might make an snprintf that receives a buffer at some point.
		write(STD_OUT, "Hi there, my pid is: ", sizeof("Hi there, my pid is: ") - 1);
		write(STD_OUT, pidString, len);
		write(STD_OUT, "\n", 1);
		usleep(interval * 5);
	}
	return 0; // Exit handled outside
}
