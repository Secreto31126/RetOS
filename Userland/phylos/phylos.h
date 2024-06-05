#include <sys.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphores.h>
#include "lib/my_lib.h"

#define MAX_PHYLOS 10
#define LEFT(i, p) (i + p - 1) % p
#define RIGHT(i, p) (i + 1) % p
#define THINKING 0
#define HUNGRY 1
#define EATING 2

void print_state();
void phylosopher(int i);
void take_forks(int i);
void put_forks(int i);
void test(int i);