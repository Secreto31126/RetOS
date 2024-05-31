#include "files.h"

static FileEmpty empties[] = {
    stdin_empty,
    stdout_empty,
    stderr_empty,
    stdkey_empty,
};

bool file_empty(int file)
{
    if (0 <= file && file < 4)
    {
        return empties[file]();
    }

    if (IS_PIPE(file))
    {
        return pipe_empty(file);
    }

    return false;
}

bool file_full(int file)
{
    if (0 <= file && file < 4)
    {
        // Internal files are never filled,
        // they overwrite old data if needed
        return false;
    }

    if (IS_PIPE(file))
    {
        return pipe_full(file);
    }

    return false;
}
