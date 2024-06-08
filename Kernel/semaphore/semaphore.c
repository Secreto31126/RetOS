#include "semaphore.h"

size_t open_sems = 0;
Semaphore semaphores[MAX_SEMS] = {};

int exists(const char *name, int *i);
int usable(const sem_t *sem);

sem_t *sem_open(const char *name, unsigned int value)
{
    if (!name || !*name)
    {
        return NULL;
    }

    int id = 0;
    if (exists(name, &id))
    {
        semaphores[id].usages++;
        return semaphores[id].sem;
    }

    if (open_sems == MAX_SEMS)
    {
        return NULL;
    }

    sem_t *new_sem = malloc(sizeof(sem_t));
    if (!new_sem)
    {
        return NULL;
    }

    strncpy(new_sem->name, name, sizeof(semaphores[id].sem->name));
    new_sem->value = value;

    semaphores[id].sem = new_sem;
    semaphores[id].usages = 1;
    open_sems++;

    return new_sem;
}

int sem_close(sem_t *sem)
{
    if (!sem)
    {
        return -1;
    }

    for (size_t i = 0; i < MAX_SEMS; i++)
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
        }
    }

    return -1;
}

int sem_unlink(const char *name)
{
    if (!name || !strlen(name))
    {
        return -1;
    }

    int i = 0;
    if (exists(name, &i))
    {
        semaphores[i].usages--;
        if (semaphores[i].usages == 0)
        {
            free(semaphores[i].sem);
            semaphores[i].sem = NULL;
            open_sems--;
        }
        return 0;
    }

    return -1;
}

int sem_post(sem_t *sem)
{
    if (!usable(sem))
    {
        return -1;
    }

    unsigned int value;
    do
    {
        // Loop trying to set the spinlock
        value = exchange(&sem->value, SPINLOCK);
    } while (SPINLOCK_LOCKED(value));

    if (value + 1 < SPINLOCK)
    {
        exchange(&sem->value, ++value);
        return 0;
    }

    return -1;
}

int sem_wait(sem_t *sem)
{
    size_t i = 0;

    if (!usable(sem))
    {
        return -1;
    }

    unsigned int value;
SPINLOCK_CHECK:
    do
    {
        // Loop trying to set the spinlock
        value = exchange(&sem->value, SPINLOCK);
    } while (SPINLOCK_LOCKED(value));

    if (!value)
    {
        // Set the spinlock to free
        exchange(&sem->value, value);
        sem_block(sem);
        goto SPINLOCK_CHECK;
    }

    exchange(&sem->value, --value);

    return 0;
}

int exists(const char *name, int *i)
{
    int free = -1;
    for (size_t j = 0; j < MAX_SEMS; j++)
    {
        if (semaphores[j].sem != NULL)
        {
            if (strncmp(semaphores[j].sem->name, name, strlen(name)) == 0)
            {
                *i = j;
                return 1;
            }
        }
        else if (free == -1)
        {
            free = j;
        }
    }

    *i = free;
    return 0;
}

int usable(const sem_t *sem)
{
    for (size_t i = 0; i < MAX_SEMS; i++)
    {
        if (semaphores[i].sem == sem)
        {
            return 1;
        }
    }

    return 0;
}
