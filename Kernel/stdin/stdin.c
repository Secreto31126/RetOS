#include "stdin.h"

static uint8_t on_your_left = 0;
static uint64_t reader = 0;
static uint64_t writer = 0;
#define INC(x) ((x) = (x)++ % BUF_SIZE)

char getc()
{
    char c = buffer[reader];
    INC(reader);
    on_your_left = 0;
    return c;
}

void putc(uint8_t c)
{
    buffer[writer] = c;
    INC(writer);

    if (on_your_left)
    {
        INC(reader);
    }
    else if (reader == writer)
    {
        on_your_left = 1;
    }
}

uint16_t read_stdin(uint8_t *buf, uint16_t len)
{
    int i = 0;
    while (i < len && (reader != writer || on_your_left))
    {
        buf[i++] = getc();
    }

    return i;
}

uint16_t write_stdin(uint8_t *buf, uint16_t len)
{
    int i = 0;
    while (i < len)
    {
        if (buf[i])
        {
            putc(buf[i++]);
        }
    }

    return i;
}
