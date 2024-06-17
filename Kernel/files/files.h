#ifndef PFLE_H
#define PFLE_H

#include <files.h>

#include <console.h>
#include <pipes.h>
#include <proc.h>
#include <stddef.h>
#include <stderr.h>
#include <stdin.h>
#include <stdkey.h>
#include <stdout.h>
#include <sys/types.h>

typedef bool (*FileEmpty)();
typedef void (*FileFlusher)();
typedef uint16_t (*FileReader)(uint8_t *, uint16_t);
typedef uint16_t (*FileWritter)(uint8_t *, uint16_t);

#endif
