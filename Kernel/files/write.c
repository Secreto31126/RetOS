#include "files.h"

static FileWritter writters[] = {
    write_stdin,
    write_stdout,
    write_stderr,
    write_stdkey,
};

ssize_t write(int fd, const void *buffer, size_t count) {
  int file = get_current_process()->files[fd];

  if (file == -1) {
    return 0;
  }

  if (0 <= file && file < 4) {
    return writters[fd]((unsigned char *)buffer, count);
  }

  if (IS_PIPE(file)) {
    return write_pipe(file, buffer, count);
  }

  return -1;
}
