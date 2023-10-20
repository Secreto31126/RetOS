#include "interruptions.h"

#define PRESSED_OR_RELEASED(x) ((x) & 0x7F)
#define IS_RELEASE(x) ((x) & 0x80)
#define RELEASED(x) ((x) | 0x80)

#define BIN(x) ((x) ? 1 : 0)
#define NEGATE(x) (1 - (x))

static void tick_handler();
static void keyboard_handler();

void pic_manager(int interrupt)
{
    switch (interrupt)
    {
    case 0:
        return tick_handler();

    case 1:
        return keyboard_handler();

    default:
        break;
    }
}

static unsigned long long tick;

static void tick_handler()
{
    if (tick++ % 18)
    {
        update_header();
    }
}

#define MAX_KEYS 17
static uint8_t pressed_keys[MAX_KEYS];

static void keyboard_handler()
{
    static int caps = 0;
    static int shift = 0;
    static int altgr = 0;
    static int event = 0;

    // Read the scancode from the keyboard controller
    uint8_t scancode = input_byte(0x60);

    // If esc key
    if (scancode == 1)
    {
        toggle_language();
        return;
    }

    // LShift or RShift pressed
    if (scancode == 0x2A || scancode == 0x36)
    {
        shift++;
        return;
    }

    // LShift or RShift released
    if (scancode == RELEASED(0x2A) || scancode == RELEASED(0x36))
    {
        shift--;
        return;
    }

    // CapsLock pressed
    if (scancode == 0x3A)
    {
        caps = BIN(!caps);
        return;
    }

    // CapsLock released
    if (scancode == RELEASED(0x3A))
    {
        return;
    }

    // Alt or AltGr pressed or released
    if (PRESSED_OR_RELEASED(scancode) == 0x38)
    {
        if (event)
        {
            altgr = BIN(!altgr);
            event = 0;
            return;
        } // Else ignore alt
    }

    if (scancode == 0xE0)
    {
        event = 1;
        return;
    }
    else if (event)
    {
        event = 0;
        return;
    }

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

    char modifier = (shift ? NEGATE(caps) : caps) + altgr * 2;
    for (int i = 0; i < MAX_KEYS; i++)
    {
        if (pressed_keys[i])
        {
            uint8_t letter = get_scancode_utf16(pressed_keys[i], modifier);
            write_stdin(&letter, 1);
        }
    }
}
