/* sampleCodeModule.c */

// char *v = (char *)0xB8000 + 79 * 2;
#include "./tbd clib/nstdlib.h"

int strlen(char *s)
{
	int i = 0;
	while (*s++)
		i++;
	return i;
}

char *ulltoa(unsigned long long ll, char *buffer, int radix)
{
	while (ll > radix)
	{
		*buffer = (ll % radix) + '0';
		if (*buffer > '9')
			*buffer += 'A' - '9' - 1;
		buffer++;
		ll /= radix;
	}

	*buffer = ll + '0';
	if (*buffer > '9')
		*buffer += 'A' - '9' - 1;

	buffer++;
	*buffer = 0;
	return buffer;
}

int main()
{
	char buffer[100];
	ulltoa(get_unix_time(), buffer, 10);
	print_sys(buffer, strlen(buffer));
	return 0xDEADC0DE;
	return 0xDEADBEEF;
}
