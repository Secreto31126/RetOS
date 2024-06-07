#include "phylos.h"
#include "random.h"

void phylosopher(unsigned int i)
{
    while (1)
    {
        take_forks(i);
        usleep(randBetween(MIN_SLEEP, MAX_SLEEP)); // Eating
        put_forks(i);
        usleep(randBetween(MIN_SLEEP, MAX_SLEEP)); // Thinking
    }
}

void set_state(int i, int state)
{
    sem_wait(data->mutex);
    data->phylos[i].state = state;
    updatePrinter(); // The printer lifts the mutex once done printing, ensures all logs are printed
}

void take_forks(unsigned int i)
{
    set_state(i, HUNGRY);
    if (i % 2)
    {
        sem_wait(data->phylos[i].sem);
        sem_wait(data->phylos[RIGHT(i, data->phylo_count)].sem);
    }
    else
    {
        sem_wait(data->phylos[RIGHT(i, data->phylo_count)].sem);
        sem_wait(data->phylos[i].sem);
    }

    set_state(i, EATING);
}

void updatePrinter()
{
    puts("."); // Tell printer that state has changed. Printer will lift mutex once done
}

void put_forks(unsigned int i)
{

    set_state(i, THINKING);

    sem_post(data->phylos[i].sem);
    sem_post(data->phylos[RIGHT(i, data->phylo_count)].sem);
}
