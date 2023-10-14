#include <stdint.h>
#include <naiveConsole.h>

uint64_t write(uint64_t fd, uint64_t buffer, uint64_t count);

uint64_t syscall_manager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax)
{
    switch (rax)
    {
    case 0:
        /* code */
        break;

    case 1:
        return write(rdi, rsi, rdx);

    default:
        break;
    }

    return -1;
}

uint64_t write(uint64_t fd, uint64_t buffer, uint64_t count)
{
    if (fd != 1)
    {
        return -1;
    }

    uint64_t i;
    char *buff = (char *)buffer;
    for (i = 0; i < count; i++)
    {
        ncPrintChar(buff[i], 0x1F);
    }

    return i;
}
