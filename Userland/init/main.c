/* main.c */

#include <string.h>
#include <sys.h>

extern char bss;
extern char endOfBinary;

#define puts(str) write(1, (str), strlen(str))

int main(int argc, char *argv[]) {
  if (fork()) {
    while (1) {
      if (waitpid(-1, NULL, 0) < 0) {
        return 0;
      }
    }
  }

  execv("module", NULL);
  return 1;
}
