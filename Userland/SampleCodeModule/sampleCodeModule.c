/* sampleCodeModule.c */

// char *v = (char *)0xB8000 + 79 * 2;
#include "nstdlib/nstdlib.h"

int main()
{
	char buffer[100];
	ultoa(get_unix_time(), buffer, 10);
	print_sys(buffer, strlen(buffer));

	printf("Miembro numero %d", 1234);

	return 0xDEADC0DE;
	return 0xDEADBEEF;
}
