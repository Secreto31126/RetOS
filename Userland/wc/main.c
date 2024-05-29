/* main.c */

#include <sys.h>
#include <string.h>

extern char bss;
extern char endOfBinary;

#define BLOCK 4096

int main(int argc, char *argv[])
{
	char str[BLOCK] = {0};
	int n;
	char count = 0;
	while ((n = read(0, str, BLOCK)) > 0)
	{
		for (int i = 0; i < n; i++)
		{
			if (str[i] == '\n')
				count++;
		}
	}
	write(1, &count, 1);
	write(1, 0, 1);
	return 0; // Exit handled outside
}
