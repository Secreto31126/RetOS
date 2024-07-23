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
  char count = 1;
  while ((n = read(STD_IN, str, BLOCK)) > 0) {
    for (int i = 0; i < n; i++) {
      if (str[i] == '\n')
        count++;
    }
  }
  itoa(count, str, 10);
  write(STD_OUT, str, strlen(str));
  char c = 0;
  write(STD_OUT, &c, 1);
  return 0; // Exit handled outside
}
