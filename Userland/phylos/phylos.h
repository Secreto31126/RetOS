#include <sys.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphores.h>
#include "lib/my_lib.h"

#define MAX_PHYLOS 15
#define INITIAL_PHYLOS 5
#define LEFT(i, p) i
#define RIGHT(i, p) (i + 1) % p
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define MIN_SLEEP 20
#define MAX_SLEEP 50
#define puts(str) write(1, (str), strlen(str))

typedef struct
{
    int state;
    sem_t *sem;
} phylo_t;

typedef struct Data
{
    sem_t *mutex;
    sem_t *printex;
    sem_t *childex;
    phylo_t phylos[MAX_PHYLOS];
    int adding;
    sem_t *addex;
    unsigned int phylo_count;
} Data;

extern Data *data;

void print_state();
void phylosopher(unsigned int i);
void take_forks(unsigned int i);
void put_forks(unsigned int i);
void leave(int count);