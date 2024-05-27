#include "interruptions.h"

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
/**
 * @brief This wrapper makes happy Lucas noises :)
 *
 * @param buffer The buffer to write to
 * @param count The number of bytes to write
 * @return uint64_t The number of bytes written
 */
static uint64_t get_lucas(uint8_t *buffer, uint64_t count);
/**
 * @brief halt once
 *
 * @param rax The value to return
 * @return uint64_t rax
 */
static uint64_t halt(uint64_t, uint64_t, uint64_t, uint64_t rax);
/**
 * @brief create a new process from the current one
 *
 * @param rsp The interruption rsp
 * @return uint64_t The new process' pid
 */
static uint64_t fork(uint64_t, uint64_t, uint64_t, uint64_t, void *rsp);

#define SYSCALL_COUNT 22
typedef uint64_t (*syscall)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
static syscall syscall_handlers[SYSCALL_COUNT] = {
    (syscall)read,
    (syscall)write,
    (syscall)draw,
    (syscall)malloc,
    (syscall)free,
    (syscall)get_time,
    (syscall)get_screen_size,
    (syscall)beep_bop,
    (syscall)get_tick,
    (syscall)get_lucas,
    (syscall)halt,
    (syscall)exit,
    (syscall)noop, // semaphore (0xCross-roads)
    (syscall)get_pid,
    (syscall)execv,
    (syscall)fork,
    (syscall)yield,
    (syscall)waitpid,
    (syscall)sleep,
    (syscall)pipe,
    (syscall)close,
    (syscall)dup2,
};

uint64_t syscall_manager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax, uint64_t rsp)
{
    if (rax < SYSCALL_COUNT)
    {
        return syscall_handlers[rax](rdi, rsi, rdx, rax, rsp);
    }

    return -1;
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

    return (get_hour_offset(hour) << 8) + minute;
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

static uint64_t get_lucas(uint8_t *buffer, uint64_t count)
{
    return read_stderr(buffer, count);
}

static uint64_t halt(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax)
{
    set_interrupt_flag();
    halt_once();
    return rax;
}

static uint64_t fork(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, void *rsp)
{
    return create_process(rsp);
}
