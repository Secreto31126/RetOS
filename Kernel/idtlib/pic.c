#include <lib.h>
#include <naiveConsole.h>

static void tick_handler();
static void keyboard_handler();

void pic_manager(int interrupt)
{
    // ncPrint("Interrupt: ");
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
    tick++;
}

static void keyboard_handler()
{
    uint8_t scancode;

    // Await the scancode from the keyboard controller
    // do
    // {
    //     scancode = input_byte(0x64);
    // } while (!(scancode & 1));

    // Read the scancode from the keyboard controller
    scancode = input_byte(0x60);

    // Do something with the scancode
    ncPrint("Hi");
}
