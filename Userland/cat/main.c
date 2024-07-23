/* main.c */

#include <stdlib.h>
#include <string.h>
#include <sys.h>

extern char bss;
extern char endOfBinary;

#define BLOCK 4096

int main(int argc, char *argv[]) {
  char str[BLOCK] = {0};
  int n;
  while ((n = read(STD_IN, str, BLOCK)) > 0) {
    write(STD_OUT, str, n);
  }
  char c = 0;
  write(STD_OUT, &c, 1);
  return 0; // Exit handled outside
}
