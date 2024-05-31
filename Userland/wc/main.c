/* main.c */

#include <sys.h>
#include <string.h>
#include <stdlib.h>

extern char bss;
extern char endOfBinary;

#define BLOCK 4096

int main(int argc, char *argv[])
{
	char str[BLOCK] = {0};
	int n;
	char count = 1;
	while ((n = read(0, str, BLOCK)) > 0)
	{
		for (int i = 0; i < n; i++)
		{
			if (str[i] == '\n')
				count++;
		}
	}
	itoa(count, str, 10);
	write(1, str, strlen(str));
	write(1, 0, 1);
	return 0; // Exit handled outside
}
