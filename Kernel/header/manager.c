#include <lib.h>
#include <idt.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <localization.h>

static void update_header_clock(char *header);
static void update_header_counter(char *header);

void update_header()
{
    char header[] = "\bRetOS\b\eExceptions: \t\t\t\t\e00:00";
    update_header_clock(header);
    update_header_counter(header);
    ncPrintHeader(header);
}

static void update_header_clock(char *header)
{
    unset_interrupt_flag();

    output_byte(0x70, 0x04);
    uint8_t hour = input_byte(0x71);

    output_byte(0x70, 0x02);
    uint8_t minute = input_byte(0x71);

    set_interrupt_flag();

    hour = get_hour_offset(hour);

    header[25] = '0' + hour / 16;
    header[26] = '0' + hour % 16;

    header[28] = '0' + minute / 16;
    header[29] = '0' + minute % 16;
}

static void update_header_counter(char *header)
{
    uint64_t exceptions = get_exceptions_count();

    while (exceptions > 0xFFFF)
        exceptions /= 16;

    int i = 23;
    do
    {
        header[i--] = '0' + exceptions % 16 + (exceptions % 16 > 9 ? 7 : 0);
    } while (exceptions /= 16 && i > 19);
}
