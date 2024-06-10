/* main.c */

#include <sys.h>
#include <string.h>
#include <stdlib.h>

extern char bss;
extern char endOfBinary;

#define READ_PER_INPUT 80
int main(int argc, char *argv[])
{
	char buffer[READ_PER_INPUT] = {0};
	int n;
	while ((n = read(STD_IN, buffer, READ_PER_INPUT - 1)) > 0)
	{
		buffer[n] = 0;
		char devNull;
		read(STD_TERM, &devNull, 1);
		write(STD_OUT, buffer, n);
	}
	return 0; // Exit handled outside
}
