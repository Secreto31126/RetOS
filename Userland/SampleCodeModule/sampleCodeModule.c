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
	uint32_t death[] = {224, 224, 458, 440, 458, 440, 476, 458, 440, 422, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 422, 476, 386, 314, 314, 224, 494, 476, 566, 566, 566, 386, 386, 224, 224, 224, 512, 494, 566, 440, 314, 404, 566, 476, 566, 314, 0};
	play_song(death, 51);

	startModules();
	return 0xDEADC0DE;
	return 0xDEADBEEF;
}
