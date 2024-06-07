#include <sys.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphores.h>
#include "lib/my_lib.h"

#define MAX_PHYLOS 15
#define LEFT(i, p) (i + p - 1) % p
#define RIGHT(i, p) (i + 1) % p
#define THINKING 0
#define EATING 1
#define puts(str) write(1, (str), strlen(str))

typedef struct
{
    int state;
    sem_t *sem;
} phylo_t;

typedef struct
{
    sem_t *mutex;
    phylo_t phylos[MAX_PHYLOS];
    unsigned int phylo_count;
} Data;

extern Data *data;

void print_state();
void phylosopher(unsigned int i);
void take_forks(unsigned int i);
void put_forks(unsigned int i);
void leave(int count);