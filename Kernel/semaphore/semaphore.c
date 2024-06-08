#include "semaphore.h"

size_t open_sems = 0;
Semaphore semaphores[MAX_SEMS] = {};

static int exists(const char *name, int *i);
static int usable(const sem_t *sem);
static int block(const sem_t *sem);
static int unblock(const sem_t *sem);
static unsigned int fkutex(sem_t *sem);

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

    strncpy(new_sem->name, name, sizeof(new_sem->name));
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
        if (semaphores[i].sem == sem)
        {
            free(semaphores[i].sem);
            semaphores[i].sem = NULL;
            open_sems--;
            return 0;
        }
    }

    return -1;
}

int sem_unlink(const char *name)
{
    if (!name || !*name)
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

    unsigned int value = fkutex(sem);
    if (value + 1 < INVALID_SEM)
    {
        if (unblock(sem))
        {
            exchange(&sem->value, value);
            return -1;
        }

        exchange(&sem->value, ++value);
        return 0;
    }

    return -1;
}

int sem_wait(sem_t *sem)
{
    if (!usable(sem))
    {
        return -1;
    }

    unsigned int value;
SPINLOCK_CHECK:
    value = fkutex(sem);
    if (!value)
    {
        // Set the spinlock to free
        exchange(&sem->value, value);
        if (block(sem))
        {
            return -1;
        }
        sched_yield();
        goto SPINLOCK_CHECK;
    }

    exchange(&sem->value, --value);

    return 0;
}

int sem_getvalue(sem_t *sem, int *sval)
{
    if (!sem || !usable(sem))
    {
        return -1;
    }

    unsigned int value = fkutex(sem);
    *sval = value & ~SPINLOCK;
    exchange(&sem->value, value);

    return 0;
}

int sem_init(sem_t *sem, int pshared, unsigned int value)
{
    if (!sem)
    {
        return -1;
    }

    if (pshared)
    {
        if (open_sems == MAX_SEMS)
        {
            return -1;
        }

        int id = 0;
        if (exists(NULL, &id))
        {
            // Weird, shouldn't happen
            return -1;
        }

        semaphores[id].sem = sem;
        semaphores[id].usages = 1;
        open_sems++;
    }

    sem->name[0] = '\0';
    sem->value = value & ~SPINLOCK;
    return 0;
}

int sem_destroy(sem_t *sem)
{
    if (!usable(sem))
    {
        return -1;
    }

    fkutex(sem);
    for (size_t i = 0; i < MAX_SEMS; i++)
    {
        if (semaphores[i].sem == sem)
        {
            semaphores[i].sem = NULL;
            open_sems--;
            break;
        }
    }

    exchange(&sem->value, INVALID_SEM);
    return 0;
}

static int exists(const char *name, int *i)
{
    int free = -1;
    for (size_t j = 0; j < MAX_SEMS; j++)
    {
        // If we aren't serching by name, return the first free slot
        if (!name)
        {
            if (semaphores[j].sem)
            {
                continue;
            }

            free = j;
            break;
        }

        if (semaphores[j].sem)
        {
            if (*semaphores[j].sem->name && strncmp(semaphores[j].sem->name, name, sizeof(semaphores[j].sem->name)) == 0)
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

static int usable(const sem_t *sem)
{
    return sem && sem->value != INVALID_SEM;
}

static bool block_condition()
{
    return false;
}

static int block(const sem_t *sem)
{
    Process *p = get_current_process();
    if (p->state != PROCESS_RUNNING)
    {
        return -1;
    }

    Semaphore *s = NULL;
    for (size_t i = 0; i < MAX_SEMS; i++)
    {
        if (semaphores[i].sem == sem)
        {
            s = semaphores + i;
            break;
        }
    }

    if (!s)
    {
        return -1;
    }

    Process *blocked = s->blocked;
    if (!blocked)
    {
        s->blocked = p;
    }
    else
    {
        while (blocked->next_blocked)
        {
            blocked = blocked->next_blocked;
        }

        blocked->next_blocked = p;
    }

    p->state = PROCESS_BLOCKED;
    p->next_blocked = NULL;
    p->block_condition = block_condition;
    p->condition_data[0] = &s->blocked;

    return 0;
}

static bool unblock_condition(pid_t pid)
{
    return true;
}

static int unblock(const sem_t *sem)
{
    if (!sem)
    {
        return -1;
    }

    Semaphore *s = NULL;
    for (size_t i = 0; i < MAX_SEMS; i++)
    {
        if (semaphores[i].sem == sem)
        {
            s = semaphores + i;
            break;
        }
    }

    if (!s)
    {
        return -1;
    }

    Process *p = s->blocked;
    if (!p)
    {
        return 0;
    }

    p->block_condition = unblock_condition;
    s->blocked = p->next_blocked;
    p->next_blocked = NULL;
    loop_blocked_and_unblock(p);

    return 0;
}

static unsigned int fkutex(sem_t *sem)
{
    unsigned int value;
    do
    {
        // Loop trying to set the spinlock
        value = exchange(&sem->value, SPINLOCK);
    } while (SPINLOCK_LOCKED(value));

    return value;
}
