#include "semaphores.h"

size_t open_sems = 0;
Semaphore semaphores[MAX_SEMS] = {};

sem_t *sem_open(const char *name, unsigned int value)
{
    size_t i = 0;
    size_t sem_id = -1;
    while (i < open_sems)
    {
        if (semaphores[i].sem != NULL)
        {
            if (strcmp(semaphores[i].sem->name, name) == 0)
            {
                semaphores[i].usages++;
                return semaphores[i].sem;
            }
            i++;
        }
        if (sem_id == -1)
        {
            sem_id = i;
        }
    }

    if (sem_id == -1 || open_sems == MAX_SEMS)
    {
        return NULL;
    }

    sem_t *new_sem = malloc(sizeof(sem_t));
    if (!new_sem)
    {
        return NULL;
    }

    strcpy(new_sem->name, name);
    new_sem->value = value;

    semaphores[sem_id].sem = new_sem;
    semaphores[sem_id].usages = 1;
    open_sems++;

    return &new_sem;
}

int sem_wait(sem_t *sem)
{
    size_t i = 0;
    while (i < open_sems)
    {
        if (semaphores[i].sem != NULL)
        {
            if (semaphores[i].sem == sem)
            {
                break;
            }
            i++;
        }
    }
    if (i == open_sems)
    {
        return -1;
    }

    while (!(sem->value & 0x80000000))
    {
    }

    int value = 0;
    value = exchange(&sem->value, value);
    while (!value)
    {
        sem_block(sem);
    }
    value--;
    sem->value += value;

    return 0;
}

int sem_post(sem_t *sem)
{
    if (sem->value >= 0x7FFFFFFF)
    {
        return -1;
    }
    sem->value++;
}

int sem_unlink(const char *name)
{
    if (!name || !strlen(name))
    {
        return -1;
    }

    for (size_t i = 0; i < open_sems;)
    {
        if (semaphores[i].sem != NULL)
        {
            if (strcmp(semaphores[i].sem->name, name) == 0)
            {
                semaphores[i].usages--;
                if (!semaphores[i].usages)
                {
                    sem_close(semaphores[i].sem);
                    semaphores[i].sem = NULL;
                    open_sems--;
                }
                return 0;
            }
            i++;
        }
    }

    return -1;
}

int sem_close(sem_t *sem)
{
    for (size_t i = 0; i < open_sems;)
    {
        if (semaphores[i].sem != NULL)
        {
            if (semaphores[i].sem == sem)
            {
                free(semaphores[i].sem);
                semaphores[i].sem = NULL;
                open_sems--;
                return 0;
            }
            i++;
        }
    }

    return -1;
}
