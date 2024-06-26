#include "lib/my_lib.h"
#include <semaphores.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys.h>

#define MAX_PHYLOS 15
#define INITIAL_PHYLOS 5
#define LEFT(i, p) i
#define RIGHT(i, p) (((i) + 1) % (p))
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define MIN_SLEEP 20
#define MAX_SLEEP 50

typedef struct {
  int state;
  sem_t *sem;
} phylo_t;

typedef struct Data {
  sem_t *mutex;
  sem_t *returnex;
  sem_t **printex;
  sem_t *childex;
  sem_t *addex;
  int adding;
  phylo_t phylos[MAX_PHYLOS];
  int phylo_count;
} Data;

extern Data *data;

/**
 * @brief Outputs the current state of the table to STD_OUT
 *
 */
void print_state();
/**
 * @brief Think, check in with the manager, nab the forks, eat, leave the forks,
 * repeat
 *
 * @param i The philosopher's position on the table
 */
void phylosopher(unsigned int i);
/**
 * @brief Waits on the forks until obtained
 *
 * @param i The philosopher's position on the table
 */
void take_forks(unsigned int i);
/**
 * @brief Returns the forks
 *
 * @param i The philosopher's position on the table
 */
void put_forks(unsigned int i);
/**
 * @brief Kills all philosophers, unlinks semaphores, frees shared memories
 *
 */
void leave();
