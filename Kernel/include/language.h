#ifndef LANG_H
#define LANG_H

#include <stdint.h>

typedef enum
{
    ENGLISH,
    SPANISH
} Language;

Language get_language();
void set_language(Language lang);
void toggle_language();

/**
 * @brief Get the scancode utf16 value
 * @details modifier is 0 for no modifier, 1 for shift and 2 for alt gr
 * @return uint16_t the scancode utf16, 0 if invalid modifier or undefined scancode
 */
uint16_t get_scancode_utf16(uint8_t, uint8_t);

#endif
