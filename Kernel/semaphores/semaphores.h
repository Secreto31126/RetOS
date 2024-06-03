#ifndef PSEM_H
#define PSEM_H

#include <semaphores.h>
#include <string.h>
#include <proc.h>

#define MAX_SEMS 20

typedef struct Semaphore
{
    sem_t sem;
    unsigned int usages;
} Semaphore;

#endif