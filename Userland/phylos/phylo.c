#include "phylos.h"

void phylosopher(unsigned int i)
{
    while (1)
    {
        take_forks(i);
        sleep(1);
        put_forks(i);
        sleep(1);
    }
}

void take_forks(unsigned int i)
{
    sem_wait(mutex); // Mutex lock
    phylos[i].state = HUNGRY;
    test(i);
    if (phylos[i].state != EATING)
    {
        phylos[i].turn = phylos[i].turn + 1;
    }
    else
    {
        phylos[i].turn = 0;
    }
    sem_post(mutex);         // Mutex unlock
    sem_wait(phylos[i].sem); // Wait until able to eat
}

void put_forks(unsigned int i)
{
    sem_wait(mutex); // Mutex lock
    phylos[i].state = THINKING;
    test(RIGHT(i, *phylo_count));
    test(LEFT(i, *phylo_count));
    sem_post(mutex); // Mutex unlock
}

void test(unsigned int i)
{
    if (phylos[i].state == HUNGRY && phylos[LEFT(i, *phylo_count)].state != EATING && phylos[RIGHT(i, *phylo_count)].state != EATING)
    {
        phylos[i].state = EATING;
        sem_post(phylos[i].sem);
    }
}
