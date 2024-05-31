/* main.c */

#include <sys.h>
#include <string.h>

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
	while ((n = read(0, str, BLOCK)) > 0)
	{
		int removed = 0;
		for (int i = 0, removed = 0; i < n; i++)
		{
			str[i - removed] = str[i];
			if (isVowel(str[i]))
				removed++;
		}
		write(1, str, n - removed);
	}
	write(1, 0, 1);
	return 0; // Exit handled outside
}
