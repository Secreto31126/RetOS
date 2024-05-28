#ifndef PPIP_H
#define PPIP_H

#include <pipes.h>
#include <stddef.h>
#include <mman.h>
#include <memory.h>
#include <proc.h>

#define MAX_PIPES 10
#define PIPE_SIZE 0x8000

typedef struct Pipe
{
    int read_ends;
    int write_ends;
    void *data;
    void *read;
    void *write;
} Pipe;

#endif