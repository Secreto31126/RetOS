/* main.c */

#include <sys.h>

int main()
{
	// return 0xDEADBEEF;

	char c;
	// beep(1000);
	while (!read(0, &c, 1))
		halt_user();
	beep(0);

	return 0xDEADC0DE;
}
