#include "localization.h"

static int gmt_offset = 0;

void set_timezone(Language lang)
{
    switch (lang)
    {
    case ES_AR:
        gmt_offset = -3;
        break;
    case FR_FR:
        gmt_offset = +2;
        break;
    case EN_US_4:
        gmt_offset = -4;
        break;
    default:
        break;
    }
}

uint16_t get_hour_offset(uint16_t hour)
{
    if (!gmt_offset)
        return hour;

    int step = gmt_offset > 0 ? 1 : -1;
    for (int i = gmt_offset; i; i -= step)
    {
        if (!hour && step < 0)
        {
            hour = 0x23;
            continue;
        }

        if (hour >= 0x23 && step > 0)
        {
            hour = 0;
            continue;
        }

        hour += step;

        if (!(hour < 0xA || (0x10 <= hour && hour < 0x1A) || (0x20 <= hour && hour < 0x2A)))
            hour += 6 * step;
    }

    return hour;
}
