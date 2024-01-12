/* main.c */

#include <sys.h>

int main()
{
	char c;
	beep(1000);
	while (!read(0, &c, 1))
		halt_user();
	beep(0);

	return 0xDEADC0DE;
}
