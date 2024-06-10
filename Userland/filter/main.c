/* main.c */

#include <sys.h>
#include <string.h>
#include <stdlib.h>

extern char bss;
extern char endOfBinary;

#define BLOCK 4096

char isVowel(char c)
{
	switch (c)
	{
	case 'a':
	case 'A':
	case 'e':
	case 'E':
	case 'i':
	case 'I':
	case 'o':
	case 'O':
	case 'u':
	case 'U':
		return 1;
		break;
	default:
		return 0;
	}
}

int main(int argc, char *argv[])
{
	char str[BLOCK] = {0};
	int n;
	while ((n = read(STD_IN, str, BLOCK)) > 0)
	{
		int removed = 0;
		for (int i = 0; i < n; i++)
		{
			str[i - removed] = str[i];
			if (isVowel(str[i]))
				removed++;
		}
		write(STD_OUT, str, n - removed - 1);
	}
	char c = 0;
	write(STD_OUT, &c, 1);
	return 0; // Exit handled outside
}
