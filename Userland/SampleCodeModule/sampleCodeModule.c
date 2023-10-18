/* sampleCodeModule.c */

// char *v = (char *)0xB8000 + 79 * 2;
#include "nstdlib/nstdlib.h"

int main()
{
	char buffer[100];
	ultoa(get_unix_time(), buffer, 10);
	print_sys(buffer, strlen(buffer));
	printf("\nMiembro numero %l letrasddsgfg\n", 1234);
	puts(itoa(2000, buffer, 10));
	printf("\npal%sra\n", "ab");

	return 0xDEADC0DE;
	return 0xDEADBEEF;
}
