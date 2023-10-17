#ifndef LANG_H
#define LANG_H

#include <stdint.h>

typedef enum
{
    EN_US_4,
    ES_AR,
    FR_FR
} Language;

/**
 * @brief Get the current OS language
 *
 * @return Language the current language
 */
Language get_language();
/**
 * @brief Set the OS language and timezone
 *
 * @param lang the language to use
 */
void set_language(Language lang);
/**
 * @brief For testing purposes
 */
void toggle_language();

/**
 * @brief Set the keyboard language layout
 */
void set_keyboard_language(Language);
/**
 * @brief Get the scancode utf16 value
 * @details modifier is 0 for no modifier, 1 for shift and 2 for alt gr
 * @return uint16_t the scancode utf16, 0 if invalid modifier or undefined scancode
 */
uint16_t get_scancode_utf16(uint8_t, uint8_t);

/**
 * @brief Set the timezone
 */
void set_timezone(Language);
/**
 * @brief Get the gmt offset value
 *
 * @return signed char the gmt offset
 */
uint16_t get_hour_offset(uint16_t);

#endif
