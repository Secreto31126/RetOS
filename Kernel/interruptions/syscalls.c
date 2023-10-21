#include "interruptions.h"

uint64_t read(uint64_t fd, char *buffer, uint64_t count);
uint64_t write(uint64_t fd, const char *buffer, uint64_t count);
/**
 * @brief Draw a pixel array to the screen via syscall
 *
 * @param figure Contiguous HexColor memory pointer to print
 * @param dimensions 0xWWWWWWWWHHHHHHHH
 * @param position 0xXXXXXXXXYYYYYYYY
 * @return uint64_t number of pixels drawn
 */
uint64_t draw(HexColor *figure, uint64_t dimensions, uint64_t position);
uint64_t mloc(uint64_t size);
uint64_t fre(uint64_t ptr);
uint64_t get_time();
/**
 * @brief Get the screen size
 *
 * @return uint64_t 0xWWWWWWWWHHHHHHHH
 */
uint64_t get_screen_size();

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
    case 334:
        return mloc(rdi);
    case 335:
        return fre(rdi);
    case 336:
        return get_time();
    case 337:
        return get_screen_size();

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

uint64_t mloc(uint64_t size)
{
    return (uint64_t)malloc(size);
}

uint64_t fre(uint64_t ptr)
{
    free((void *)ptr);
    return 0;
}

uint64_t get_time()
{
    unset_interrupt_flag();

    output_byte(0x70, 0x04);
    uint8_t hour = input_byte(0x71);

    output_byte(0x70, 0x02);
    uint8_t minute = input_byte(0x71);

    set_interrupt_flag();

    return hour << 8 + minute;
}

uint64_t get_screen_size()
{
    return get_width() << 16 + get_height();
}
