#ifndef PFLE_H
#define PFLE_H

#include <files.h>

#include <sys/types.h>
#include <console.h>
#include <stddef.h>
#include <stdin.h>
#include <stdout.h>
#include <stderr.h>
#include <stdkey.h>
#include <pipes.h>
#include <proc.h>

typedef bool (*FileEmpty)();
typedef void (*FileFlusher)();
typedef uint16_t (*FileReader)(uint8_t *, uint16_t);
typedef uint16_t (*FileWritter)(uint8_t *, uint16_t);

#endif
