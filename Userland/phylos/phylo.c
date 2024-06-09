#include "phylos.h"
#include "random.h"

void check_adding(unsigned int i);
void phylosopher(unsigned int i)
{
    sem_wait(data->mutex);
    sem_post(data->mutex); // wait for other phylosophers to be born
    while (1)
    {
        check_adding(i); // See if manager wants to add a philosopher to my right
        take_forks(i);
        usleep(randBetween(MIN_SLEEP, MAX_SLEEP)); // Eating
        put_forks(i);
        usleep(randBetween(MIN_SLEEP, MAX_SLEEP)); // Thinking
    }
}

void updatePrinter()
{
    sem_post(data->printex); // Tell printer that state has changed. Printer will lift mutex once done
}

void set_state(int i, int state)
{
    sem_wait(data->mutex);
    data->phylos[i].state = state;
    updatePrinter(); // The printer lifts the mutex once done printing, ensures all logs are printed
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
    set_state(i, HUNGRY);
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

    set_state(i, EATING);
}

void put_forks(unsigned int i)
{

    set_state(i, THINKING);

    sem_post(data->phylos[LEFT(i, data->phylo_count)].sem);
    sem_post(data->phylos[RIGHT(i, data->phylo_count)].sem);
}
