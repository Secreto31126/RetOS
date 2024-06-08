#ifndef PSEM_H
#define PSEM_H

#include <semaphores.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <proc.h>
#include <malloc.h>
#include <sched.h>
#include <lib.h>

#define MAX_SEMS 30

#define SPINLOCK (1 << (sizeof(unsigned int) * 8 - 1))
#define SPINLOCK_LOCKED(x) ((x) & SPINLOCK)

#define INVALID_SEM (~SPINLOCK)

typedef struct Semaphore
{
    sem_t *sem;
    unsigned int usages;
    Process *blocked;
} Semaphore;

#endif
