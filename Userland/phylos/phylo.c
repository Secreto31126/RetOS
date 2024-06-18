#include "phylos.h"
#include "random.h"

#define puts(str) write(1, (str), strlen(str))

void check_adding(unsigned int i);
void phylosopher(unsigned int i) {
  while (1) {
    usleep(randBetween(MIN_SLEEP, MAX_SLEEP)); // Thinking
    check_adding(i); // See if manager wants to add a philosopher to my right (I
                     // can't be holding a fork when that happens)
    take_forks(i);
    usleep(randBetween(MIN_SLEEP, MAX_SLEEP)); // Eating
    put_forks(i);
  }
}

/**
 * @brief Wait on the state update mutex, record a change in the philosopher's
 * state, and post the printer's semaphore
 *
 * @param i The philosopher's position on the table
 * @param state The state
 */
void set_state(int i, int state) {
  sem_wait(data->printex[0]); // The state update mutex
  data->phylos[i].state = state;
  sem_post(data->printex[1]); // Tell printer that state has changed. Printer
                              // will lift printex once done
}

/**
 * @brief Check whether the manager wants to add or remove a philosopher
 *
 * @param i The philosopher's position on the table
 */
void check_adding(unsigned int i) {
  if (data->adding == i) {
    sem_post(data->addex);
    sem_wait(data->mutex);
    sem_post(data->mutex);
    sem_post(data->returnex);
  }
}

void take_forks(unsigned int i) {
  set_state(i, HUNGRY);
  if (i % 2) {
    sem_wait(data->phylos[LEFT(i, data->phylo_count)].sem);
    sem_wait(data->phylos[RIGHT(i, data->phylo_count)].sem);
  } else {
    sem_wait(data->phylos[RIGHT(i, data->phylo_count)].sem);
    sem_wait(data->phylos[LEFT(i, data->phylo_count)].sem);
  }

  set_state(i, EATING);
}

void put_forks(unsigned int i) {

  set_state(i, THINKING);

  sem_post(data->phylos[LEFT(i, data->phylo_count)].sem);
  sem_post(data->phylos[RIGHT(i, data->phylo_count)].sem);
}
