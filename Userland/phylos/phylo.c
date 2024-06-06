#include "phylos.h"
#include "random.h"

void phylosopher(unsigned int i)
{
    while (1)
    {
        take_forks(i);
        usleep(randBetween(MIN_SLEEP, MAX_SLEEP)); // comer
        put_forks(i);
        usleep(randBetween(MIN_SLEEP, MAX_SLEEP)); // pensar
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

void updatePrinter()
{
    puts(".");       // Tell printer that state has changed. Printer will lift mutex once done
    sem_wait(mutex); // Recover mutex
    // It is fine to lose the mutex in between tests: As phylo is THINKING, its state will not be changed by another, while all other states are checked safely within test, inside a mutex
}

void put_forks(unsigned int i)
{
    sem_wait(mutex); // Mutex lock
    phylos[i].state = THINKING;
    updatePrinter();
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
        updatePrinter();
    }
}
