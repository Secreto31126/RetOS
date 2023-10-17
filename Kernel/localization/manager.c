#include "localization.h"

static Language current_language;

Language get_language()
{
    return current_language;
}

void set_language(Language lang)
{
    current_language = lang;
    set_keyboard_language(lang);
    set_timezone(lang);
}

void toggle_language()
{
    Language lang;
    switch (current_language)
    {
    case ES_AR:
        lang = FR_FR;
        break;

    case FR_FR:
        lang = EN_US_4;
        break;

    default:
        lang = ES_AR;
        break;
    }

    set_language(lang);
}
