#include "interruptions.h"

/**
 * @brief Read from a file descriptor
 *
 * @param fd 0 for stdin, 3 for stdkey
 * @param buffer The buffer to write to
 * @param count The number of bytes to read
 * @return uint64_t The number of bytes read
 */
static uint64_t read(uint64_t fd, char *buffer, uint64_t count);
static uint64_t write(uint64_t fd, const char *buffer, uint64_t count);
/**
 * @brief Draw a pixel array to the screen via syscall
 *
 * @param figure Contiguous HexColor memory pointer to draw
 * @param dimensions 0xWWWWHHHH
 * @param position 0xXXXXYYYY
 * @return uint64_t number of pixels drawn
 */
static uint64_t draw(HexColor *figure, uint32_t dimensions, uint32_t position);
static uint64_t get_time();
/**
 * @brief Get the screen size
 *
 * @return uint32_t 0xWWWWHHHH
 */
static uint32_t get_screen_size();
/**
 * @brief Let it beep
 *
 * @param rdi The sound frequency
 * @param _ Unused
 * @param _ Unused
 * @param rax The syscall number, it's the return value to avoid modifying the register
 * @return uint64_t rax
 */
static uint64_t beep_bop(uint64_t rdi, uint64_t, uint64_t, uint64_t rax);

#define SYSCALL_COUNT 10
typedef uint64_t (*syscall)(uint64_t, uint64_t, uint64_t, uint64_t);
static syscall syscall_handlers[SYSCALL_COUNT] = {
    read,
    write,
    draw,
    malloc,
    free,
    get_time,
    get_screen_size,
    beep_bop,
    get_tick,
    add_task,
};

uint64_t syscall_manager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax)
{
    if (rax < SYSCALL_COUNT)
    {
        return syscall_handlers[rax](rdi, rsi, rdx, rax);
    }

    return -1;
}

static uint64_t (*files[4])(uint8_t *, uint16_t) = {
    read_stdin,
    noop,
    noop,
    read_stdkey,
};

static uint64_t read(uint64_t fd, char *buffer, uint64_t count)
{
    if (fd < 4)
        return files[fd]((uint8_t *)buffer, count);
    return -1;
}

static uint64_t write(uint64_t fd, const char *buffer, uint64_t count)
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

static uint64_t draw(HexColor *figure, uint32_t dimensions, uint32_t position)
{
    uint32_t width = (dimensions >> 16) & 0xFFFF;
    uint32_t height = dimensions & 0xFFFF;

    uint32_t x = (position >> 16) & 0xFFFF;
    uint32_t y = position & 0xFFFF;

    return drawFromArray(figure, width, height, x, y);
}

static uint64_t get_time()
{
    unset_interrupt_flag();

    output_byte(0x70, 0x04);
    uint8_t hour = input_byte(0x71);

    output_byte(0x70, 0x02);
    uint8_t minute = input_byte(0x71);

    set_interrupt_flag();

    return hour << 8 + minute;
}

static uint32_t get_screen_size()
{
    return (get_width() << 16) + get_height();
}

static uint64_t beep_bop(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax)
{
    beep(rdi);
    return rax;
}
