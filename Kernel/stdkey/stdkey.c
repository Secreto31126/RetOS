#include "stdkey.h"

static uint8_t on_your_left = 0;
static uint64_t reader = 0;
static uint64_t writer = 0;
#define INC(x) ((x) = ((x) + 1) % BUF_SIZE)

#define PRESSED_OR_RELEASED(x) ((x) & 0x7F)
#define IS_RELEASE(x) ((x) & 0x80)

#define MAX_KEYS 17
static uint8_t pressed_keys[MAX_KEYS];

static char getc()
{
    char c = buffer[reader];
    INC(reader);
    on_your_left = 0;
    return c;
}

static void putc(uint8_t c)
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

uint16_t read_stdkey(uint8_t *buf, uint16_t len)
{
    int i = 0;
    while (i < len && (reader != writer || on_your_left))
    {
        buf[i++] = getc();
    }

    return i;
}

uint16_t write_stdkey(uint8_t *buf, uint16_t len)
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

void stdkey_keyevent(uint8_t scancode)
{
    // If the scancode is a release event, release the key from the pressed keys array
    if (IS_RELEASE(scancode))
    {
        uint8_t code = PRESSED_OR_RELEASED(scancode);
        for (int i = 0; i < MAX_KEYS; i++)
        {
            if (code == pressed_keys[i])
            {
                pressed_keys[i] = 0;
                break;
            }
        }
    }
    else
    {
        int pos = -1;
        for (int i = 0; i < MAX_KEYS; i++)
        {
            if (pressed_keys[i] == scancode)
            {
                pos = -1;
                break;
            }

            if (pos < 0 && pressed_keys[i] == 0)
            {
                pos = i;
            }
        }

        if (pos >= 0)
        {
            pressed_keys[pos] = PRESSED_OR_RELEASED(scancode);
        }
    }

    for (int i = 0; i < MAX_KEYS; i++)
    {
        if (pressed_keys[i])
        {
            write_stdkey(&pressed_keys[i], 1);
        }
    }
}

bool stdkey_empty()
{
    return reader == writer && !on_your_left;
}

void flush_stdkey()
{
    reader = writer;
    on_your_left = 0;
}
