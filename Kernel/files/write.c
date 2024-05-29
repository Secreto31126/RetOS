#include "files.h"

static FileWritter writters[] = {
    write_stdin,
    write_stdout,
    write_stderr,
    write_stdkey,
};

size_t write(int fd, const void *buffer, size_t count)
{
    int file = get_current_process()->files[fd];

    while (file_full(file))
    {
        write_block(file);
    }

    if (file < 4)
    {
        return writters[fd]((unsigned char *)buffer, count);
    }
    else if (IS_PIPE(file))
    {
        return write_pipe(file, buffer, count);
    }

    return -1;
}
