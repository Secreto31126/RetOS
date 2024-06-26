#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

/**
 * @brief Prints a string in the screen as the header
 *
 * @deprecated It was fun while it lasted
 *
 * @param string The string to be printed with custom format,
 * \\b for begining, \\n for in the middle, \\e for end,
 * \\t for trash
 */
void ncPrintHeader(const char *string);
void ncPrint(const char *string);
void ncPrintChar(char character, char color);
void ncNewline();
void ncDeleteChar();
void ncTab();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();

#endif
