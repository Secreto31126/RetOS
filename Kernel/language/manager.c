#include "language.h"

static Language current_language;

Language get_language()
{
    return current_language;
}

void set_language(Language lang)
{
    set_keyboard_language(lang);
}

void toggle_language()
{
    if (current_language == ENGLISH)
        set_language(SPANISH);
    else
        set_language(ENGLISH);
}
