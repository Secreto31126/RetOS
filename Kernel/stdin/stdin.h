#ifndef PSTDIN_H
#define PSTDIN_H

#include <stdin.h>

static uint8_t *const buffer = (uint8_t *)0xA0000; // stdin is stored in non color text mode
#define BUF_SIZE 0x10000

#endif
