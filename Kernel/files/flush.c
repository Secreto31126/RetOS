#include "files.h"

static FileFlusher readers[] = {
    flush_stdin,
    flush_stdout,
    flush_stderr,
    flush_stdkey,
};

int flush(int fd) {
  int file = get_current_process()->files[fd];

  if (0 <= file && file < 4) {
    readers[file]();
    return 0;
  }

  return -1;
}
