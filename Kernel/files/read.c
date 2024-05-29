#include "files.h"

static FileReader readers[] = {
    read_stdin,
    read_stdout,
    read_stderr,
    read_stdkey,
};

size_t read(int fd, void *buffer, size_t count)
{
    int file = get_current_process()->files[fd];

    while (file_empty(file))
    {
        read_block(file);
    }

    if (file < 4)
    {
        return readers[fd]((unsigned char *)buffer, count);
    }
    else if (IS_PIPE(file))
    {
        return read_pipe(file, buffer, count);
    }

    return -1;
}
