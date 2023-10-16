#ifndef PLAN_H
#define PLAN_H

#include <stdint.h>
#include <language.h>

#define LAZY_KEYBOARD_LAYOUT_LIMIT 58

/**
 * @brief A table of conversion from scancode to UTF16 code for the selected
 * keyboard layout
 *
 * @details keyboard_scancodes[scaned_code][modifier], where modifier is 0 for no
 * modifier, 1 for shift and 2 for alt gr
 */
static uint16_t const (*keyboard_scancodes)[3];

/**
 * @brief Set the keyboard language layout
 */
void set_keyboard_language(Language);

#endif
