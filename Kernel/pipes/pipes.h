#ifndef PPIP_H
#define PPIP_H

#include <memory.h>
#include <mman.h>
#include <pipes.h>
#include <proc.h>
#include <semaphores.h>
#include <stddef.h>
#include <unistd.h>

#define MAX_PIPES 100
#define PIPE_SIZE 0x8000

typedef struct Pipe {
  int read_ends;
  int write_ends;
  void *data;
  void *read;
  void *write;
  sem_t read_sem;
  sem_t write_sem;
} Pipe;

#endif
