#include "semaphores.h"

size_t open_sems = 0;
Semaphore semaphores[MAX_SEMS] = {};

int init_sem(sem_t *sem, unsigned int value)
{
    if (open_sems >= MAX_SEMS)
    {
        return -1;
    }

    size_t sem_id = 0;
    while (sem_id < MAX_SEMS && semaphores[sem_id].usages)
    {
        sem_id++;
    }

    sem_t new_sem = {
        .name = "",
        .value = value + 0x80000000,
    };

    semaphores[sem_id].sem = new_sem;
    semaphores[sem_id].usages = 1;
    open_sems++;

    return 0;
}

sem_t *sem_open(const char *name, unsigned int value)
{
    for (size_t i = 0; i < MAX_SEMS; i++)
    {
        if (strcmp(semaphores[i].sem.name, name) == 0)
        {
            semaphores[i].usages++;
            return &semaphores[i].sem;
        }
    }

    if (open_sems >= MAX_SEMS)
    {
        return NULL;
    }

    size_t sem_id = 0;
    while (sem_id < MAX_SEMS && semaphores[sem_id].usages)
    {
        sem_id++;
    }

    sem_t new_sem = {
        .name = name,
        .value = value + 0x80000000,
    };

    semaphores[sem_id].sem = new_sem;
    semaphores[sem_id].usages = 1;
    open_sems++;

    return &new_sem;
}

int sem_wait(sem_t *sem)
{
    while (sem->value & 0x80000000)
    {
    }

    int value = 0;
    value = exchange(&sem->value, value);
    while (!value)
    {
        // TODO: block
    }
    value--;
    sem->value += value;

    return 0;
}

int sem_post(sem_t *sem)
{
    sem->value++;
}

int sem_unlink(const char *name)
{
    for (size_t i = 0; i < MAX_SEMS; i++)
    {
        if (strcmp(semaphores[i].sem.name, name) == 0)
        {
            semaphores[i].usages--;
            if (!semaphores[i].usages)
            {
                semaphores[i] = (Semaphore){};
            }
            return 0;
        }
    }

    return -1;
}

int close_sem(sem_t *sem)
{
    for (size_t i = 0; i < MAX_SEMS; i++)
    {
        if (&semaphores[i].sem == sem)
        {
            semaphores[i] = (Semaphore){};
            return 0;
        }
    }

    return -1;
}