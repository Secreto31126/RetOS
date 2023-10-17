#include "interruptions.h"

uint64_t read(uint64_t fd, char *buffer, uint64_t count);
uint64_t write(uint64_t fd, const char *buffer, uint64_t count);

uint64_t syscall_manager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax)
{
    switch (rax)
    {
    case 0:
        return read(rdi, (char *)rsi, rdx);
    case 1:
        return write(rdi, (const char *)rsi, rdx);

    default:
        break;
    }

    return -1;
}

uint64_t read(uint64_t fd, char *buffer, uint64_t count)
{
    memcpy(buffer, "Hello World!\n", count);
    return count;
}

uint64_t write(uint64_t fd, const char *buffer, uint64_t count)
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
