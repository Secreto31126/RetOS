#include "phylos.h"

void phylosopher(unsigned int i)
{
    while (1)
    {
        take_forks(i);
        sleep(1); // Eating
        put_forks(i);
        sleep(1); // Thinking
    }
}

void take_forks(unsigned int i)
{
    if (i % 2)
    {
        sem_wait(data->phylos[i].sem);
        sem_wait(data->phylos[RIGHT(i, data->phylo_count)].sem);
    }
    else
    {
        sem_wait(data->phylos[RIGHT(i, data->phylo_count)].sem);
        sem_wait(data->phylos[LEFT(i, data->phylo_count)].sem);
    }
    data->phylos[i].state = EATING;
}

void put_forks(unsigned int i)
{
    data->phylos[i].state = THINKING;
    sem_post(data->phylos[i].sem);
    sem_post(data->phylos[RIGHT(i, data->phylo_count)].sem);
}
