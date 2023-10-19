#include "stdin.h"

static uint64_t reader = 0;
static uint64_t writer = 0;

char getc()
{
    char c = buffer[reader];
    reader = reader++ % BUF_SIZE;
    return c;
}

void putc(uint8_t c)
{
    buffer[writer] = c;
    writer = writer++ % BUF_SIZE;
}

void read_stdin(uint8_t *buf, uint16_t len)
{
    int i = 0;
    while (i < len)
    {
        buf[i++] = getc();
    }
}

void write_stdin(uint8_t *buf, uint16_t len)
{
    int i = 0;
    while (i < len)
    {
        if (buf[i])
        {
            putc(buf[i++]);
        }
    }
}
