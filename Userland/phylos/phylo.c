#include "phylos.h"
void check_adding(unsigned int i);
void phylosopher(unsigned int i)
{
    while (1)
    {
        check_adding(i); // See if manager wants to add a philosopher to my right
        take_forks(i);
        sleep(1); // Eating
        put_forks(i);
        sleep(1); // Thinking
    }
}

void check_adding(unsigned int i)
{
    if (data->adding == i)
    {
        puts("Here");
        sem_post(data->addex);
        sem_wait(data->mutex);
        sem_post(data->mutex);
    }
}

void take_forks(unsigned int i)
{
    if (i % 2)
    {
        sem_wait(data->phylos[LEFT(i, data->phylo_count)].sem);
        sem_wait(data->phylos[RIGHT(i, data->phylo_count)].sem);
    }
    else
    {
        sem_wait(data->phylos[RIGHT(i, data->phylo_count)].sem);
        sem_wait(data->phylos[LEFT(i, data->phylo_count)].sem);
    }

    sem_wait(data->mutex);
    data->phylos[i].state = EATING;
    sem_post(data->mutex);
}

void put_forks(unsigned int i)
{
    sem_wait(data->mutex);
    data->phylos[i].state = THINKING;
    sem_post(data->mutex);

    sem_post(data->phylos[LEFT(i, data->phylo_count)].sem);
    sem_post(data->phylos[RIGHT(i, data->phylo_count)].sem);
}
