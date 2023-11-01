#ifndef PSTDIN_H
#define PSTDIN_H

#include <stdin.h>

static uint8_t *const buffer = (uint8_t *)0x70000;
#define BUF_SIZE 0x10000

#endif
