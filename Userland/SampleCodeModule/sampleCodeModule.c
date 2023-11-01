/* sampleCodeModule.c */

// char *v = (char *)0xB8000 + 79 * 2;
#include "nstdlib/nstdlib.h"
#include "shell/shell.h"
#include "snake/snake.h"
#include "moduleSelector/moduleSelector.h"

int main()
{
	/*
	printf("\nThis %s a %s long %s. The number here is -4*2:%d and here is 10*10*10:%u and this one is 456:%l. You will see some percentages here: % % %o %% \n", "is", "kinda", "string", -4 * 2, 10 * 10 * 10, 456);
	printf("\nYou will see a five here if this works: %d", pow(2.5, 2));
	for (int i = 0; i < 10; i++)
		printf("%c", getChar());
	*/
	uint32_t death[] = {0, 20556, 18108, 15642, 11088, 6786, 180, 36, 10188, 558, 90, 2016, 10656, 12852, 1476, 810, 1476, 6372, 9054, 72, 16830, 5868, 2196, 2430, 1602, 216, 16578, 90, 4986, 1386, 20556, 2070, 0, 8658, 8712, 1062, 2718, 3222, 1026, 10656, 6498, 4788, 1926, 8550, 13374, 16902, 6030, 432, 3330, 108, 0};
	play_song(death, 50);

	startModules();
	return 0xDEADC0DE;
	return 0xDEADBEEF;
}
