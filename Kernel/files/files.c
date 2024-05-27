#include "files.h"

static FileEmpty empties[] = {
    stdin_empty,
    stdout_empty,
    stderr_empty,
    stdkey_empty,
};

bool would_block(int file)
{
    if (file < 4)
    {
        return empties[file]();
    }
    else if (IS_PIPE(file))
    {
        return pipe_empty(file);
    }

    return false;
}
