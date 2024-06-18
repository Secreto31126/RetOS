/* main.c */

#include <stdlib.h>
#include <string.h>
#include <sys.h>

extern char bss;
extern char endOfBinary;

#define BLOCK 4096
// Note, doesn't quite work with split reads
int main(int argc, char *argv[]) {
  if (argc < 0) {
    write(STD_OUT, "No matching string provided.",
          sizeof("No matching string provided") - 1);
    return 1;
  }

  int n, remaining = 0;
  char buffer[BLOCK], *match = argv[0], fitFlag = 0, doEnter = 1;

  while ((n = read(STD_IN, buffer + remaining, BLOCK - 1 - remaining)) > 0) {
    buffer[n] = 0;
    int i = 0, currentStartIndex = 0;
    fitFlag = 0;

    for (; buffer[i]; i++) {

      if (buffer[i] == '\n') {
        buffer[i] = 0;

        if (doEnter) {
          doEnter = 0;
          write(STD_OUT, "\n", 1);
        }

        while (buffer[i] == '\n') {
          i++;
        }

        fitFlag = 1;

        if (strstr(buffer + currentStartIndex, match) != NULL) {
          write(STD_OUT, buffer + currentStartIndex, i - currentStartIndex);
          doEnter = 1;
        }
        currentStartIndex = i + 1;
      }
    }

    // if any parsing occurred, move what was not parsed to the start of the
    // buffer
    if (fitFlag) {
      if (*(buffer + currentStartIndex))
        strcat(buffer, buffer + currentStartIndex);
      else
        *buffer = 0;
    }
    // if the buffer did not have a single line divider, treat it as a line. The
    // only alternative is using malloc, which would make this function
    // unreadable and unwieldly (and a terrible waste of time)
    else if (strstr(buffer + currentStartIndex, match) != NULL) {
      write(STD_OUT, buffer + currentStartIndex, i - currentStartIndex);
    }

    remaining = n - currentStartIndex;
  }

  // last sentence is whatever remains on the buffer
  if (strstr(buffer, match) != NULL) {
    write(STD_OUT, buffer, strlen(buffer));
  }

  return 0; // Exit handled outside
}
