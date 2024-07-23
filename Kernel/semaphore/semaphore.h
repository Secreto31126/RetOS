#ifndef PSEM_H
#define PSEM_H

#include <lib.h>
#include <malloc.h>
#include <proc.h>
#include <sched.h>
#include <semaphores.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define MAX_SEMS 300

#define SPINLOCK (1 << (sizeof(unsigned int) * 8 - 1))
#define SPINLOCK_LOCKED(x) ((x)&SPINLOCK)

#define INVALID_SEM (~SPINLOCK)

typedef struct Semaphore {
  sem_t *sem;
  unsigned int usages;
  Process *blocked;
} Semaphore;

#endif
