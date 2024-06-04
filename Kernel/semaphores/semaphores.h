#ifndef PSEM_H
#define PSEM_H

#include <semaphores.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <proc.h>
#include <malloc.h>

#define MAX_SEMS 30

#define SPINLOCK 0x80000000
#define SPINLOCK_LOCKED(x) ((x) & SPINLOCK)

typedef struct Semaphore
{
    sem_t *sem;
    unsigned int usages;
} Semaphore;

#endif
