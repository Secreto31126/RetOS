/**
 * @brief A cool header manager for text mode
 * @deprecated It was fun while it lasted
 */

#include <lib.h>
#include <interruptions.h>
#include <stdint.h>
#include <console.h>
#include <localization.h>

static void update_header_clock(char *header);
// static void update_header_counter(char *header);

void update_header()
{
    char header[] = "\bRetOS\b\eExceptions: DEAD\e00:00";
    update_header_clock(header);
    // update_header_counter(header);
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

// static void update_header_counter(char *header)
// {
//     uint16_t count = get_exceptions_count();

//     int i = 23;
//     do
//     {
//         header[i--] = '0' + count % 16 + (count % 16 > 9 ? 7 : 0);
//     } while (count /= 16 && i > 19);
// }
