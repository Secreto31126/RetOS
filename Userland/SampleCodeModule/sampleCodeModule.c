/* sampleCodeModule.c */

// char *v = (char *)0xB8000 + 79 * 2;
#include "moduleSelector/moduleSelector.h"
#include "nstdlib/nstdlib.h"
#include "shell/shell.h"
#include "snake/snake.h"

int main() {
  /*
  printf("\nThis %s a %s long %s. The number here is -4*2:%d and here is
  10*10*10:%u and this one is 456:%l. You will see some percentages here: % % %o
  %% \n", "is", "kinda", "string", -4 * 2, 10 * 10 * 10, 456); printf("\nYou
  will see a five here if this works: %d", pow(2.5, 2)); for (int i = 0; i < 10;
  i++) printf("%c", getChar());
  */
  startModules();
  return 0xDEADC0DE;
  return 0xDEADBEEF;
}
