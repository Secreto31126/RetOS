#include "interruptions.h"

uint64_t read(uint64_t fd, char *buffer, uint64_t count);
uint64_t write(uint64_t fd, const char *buffer, uint64_t count);
/**
 * @brief Draw a pixel array to the screen via syscall
 *
 * @param figure Contiguous HexColor memory pointer to print
 * @param dimensions 0xWWWWWWWWHHHHHHHH
 * @param position 0xXXXXXXXXYYYYYYYY
 */
uint64_t draw(HexColor *figure, uint64_t dimensions, uint64_t position);

uint64_t syscall_manager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax)
{
    switch (rax)
    {
    case 0:
        return read(rdi, (char *)rsi, rdx);
    case 1:
        return write(rdi, (const char *)rsi, rdx);
    case 333:
        return draw((HexColor *)rdi, rsi, rdx);

    default:
        break;
    }

    return -1;
}

uint64_t read(uint64_t fd, char *buffer, uint64_t count)
{
    if (fd != 0)
    {
        return -1;
    }

    return read_stdin((uint8_t *)buffer, count);
}

uint64_t write(uint64_t fd, const char *buffer, uint64_t count)
{
    if (fd != 1)
    {
        return -1;
    }

    uint64_t i;
    for (i = 0; i < count; i++)
    {
        ncPrintChar(buffer[i], 0x0F);
    }

    return i;
}

uint64_t draw(HexColor *figure, uint64_t dimensions, uint64_t position)
{
    uint32_t width = dimensions >> 32 & 0xFFFFFFFF;
    uint32_t height = dimensions & 0xFFFFFFFF;

    uint32_t x = position >> 32 & 0xFFFFFFFF;
    uint32_t y = position & 0xFFFFFFFF;

    return drawFromArray(figure, width, height, x, y);
}
