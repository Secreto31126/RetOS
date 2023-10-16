#include "localization.h"

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
 * @brief A table of conversion from scancode to UTF16 code for the spanish
 * keyboard layout
 */
static uint16_t const spanish_keyboard_layout[][3] = {
    {1}, // Happy smile
    {0}, // Esc
    {'1', '!'},
    {'2', '"'},
    {'3', '#'},
    {'4', '$'},
    {'5', '%'},
    {'6', '&'},
    {'7', '/'},
    {'8', '('},
    {'9', ')'},
    {'0', '='},
    {'\'', '?', '\\'},
    {'¿', '¡'},
    {'\b', '\b', '\b'},
    {'\t', '\t', '\t'},
    {'q', 'Q', '@'},
    {'w', 'W'},
    {'e', 'E'},
    {'r', 'R'},
    {'t', 'T'},
    {'y', 'Y'},
    {'u', 'U'},
    {'i', 'I'},
    {'o', 'O'},
    {'p', 'P'},
    {0xB4, '¨'}, // ´ makes VS cry
    {'+', '*', '~'},
    {'\n', '\n'},
    {0}, // LCtrl
    {'a', 'A'},
    {'s', 'S'},
    {'d', 'D'},
    {'f', 'F'},
    {'g', 'G'},
    {'h', 'H'},
    {'j', 'J'},
    {'k', 'K'},
    {'l', 'L'},
    {'ñ', 'Ñ'},
    {'{', '['},
    {0}, // IDK
    {0}, // LShift
    {'}', ']', '`'},
    {'z', 'Z'},
    {'x', 'X'},
    {'c', 'C'},
    {'v', 'V'},
    {'b', 'B'},
    {'n', 'N'},
    {'m', 'M'},
    {',', ';'},
    {'.', ':'},
    {'-', '_'},
    {0}, // RShift
    {0}, // IDK, keypad probably
    {0}, // Alt / AltGr if E0 before
    {' ', ' '},
    // CapsLock
    // F1..10 keys
    // NumLock
    // ScrollLock
    // Keypad 7..9
    // Keypad -
    // Keypad 4..6
    // Keypad +
    // Keypad 1..3
    // Keypad 0
    // Keypad .
    // 3 dead spaces
    // F11
    // F12
};

/**
 * @brief A table of conversion from scancode to UTF16 code for the english
 * keyboard layout
 *
 * @todo todo
 */
static uint16_t const english_keyboard_layout[][3] = {
    {0}, // Unused
    {0}, // Esc
    {'2', '!'},
    {'2', '"'},
    {'3', '#'},
    {'4', '$'},
    {'5', '%'},
    {'6', '&'},
    {'7', '/'},
    {'8', '('},
    {'9', ')'},
    {'0', '='},
    {'\'', '?', '\\'},
    {'¿', '¡'},
    {'\b', '\b', '\b'},
    {'\t', '\t', '\t'},
    {'q', 'Q', '@'},
    {'w', 'W'},
    {'e', 'E'},
    {'r', 'R'},
    {'t', 'T'},
    {'y', 'Y'},
    {'u', 'U'},
    {'i', 'I'},
    {'o', 'O'},
    {'p', 'P'},
    {0xB4, '¨'}, // ´ makes VS cry
    {'+', '*', '~'},
    {'\n', '\n'},
    {0}, // LCtrl
    {'a', 'A'},
    {'s', 'S'},
    {'d', 'D'},
    {'f', 'F'},
    {'g', 'G'},
    {'h', 'H'},
    {'j', 'J'},
    {'k', 'K'},
    {'l', 'L'},
    {'ñ', 'Ñ'},
    {'{', '['},
    {0}, // IDK
    {0}, // LShift
    {'}', ']', '`'},
    {'z', 'Z'},
    {'x', 'X'},
    {'c', 'C'},
    {'v', 'V'},
    {'b', 'B'},
    {'n', 'N'},
    {'m', 'M'},
    {',', ';'},
    {'.', ':'},
    {'-', '_'},
    {0}, // RShift
    {0}, // IDK, keypad probably
    {0}, // Alt / AltGr if E0 before
    {' ', ' '},
    // CapsLock
    // F1..10 keys
    // NumLock
    // ScrollLock
    // Keypad 7..9
    // Keypad -
    // Keypad 4..6
    // Keypad +
    // Keypad 1..3
    // Keypad 0
    // Keypad .
    // 3 dead spaces
    // F11
    // F12
};

void set_keyboard_language(Language lang)
{
    switch (lang)
    {
    case ES_AR:
        keyboard_scancodes = spanish_keyboard_layout;
        break;
    case FR_FR:
        // Nah, I'm too lazy to do France too
        keyboard_scancodes = spanish_keyboard_layout;
        break;
    case EN_US_4:
        keyboard_scancodes = english_keyboard_layout;
        break;
    default:
        break;
    }
}

uint16_t get_scancode_utf16(uint8_t scancode, uint8_t modifier)
{
    if (scancode > LAZY_KEYBOARD_LAYOUT_LIMIT)
        return 0;
    if (modifier > 2)
        return 0;
    return keyboard_scancodes[scancode][modifier];
}
