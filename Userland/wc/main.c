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
	while ((n = read(0, str, BLOCK)) > 0)
	{
		write(1, str, n);
	}
	write(1, 0, 1);
	return 0; // Exit handled outside
}
