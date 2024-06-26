/* main.c */
#include "lib/my_lib.h"
#include "phylos.h"
#include "random.h"
#include <stdio.h>
#include <stdlib.h>

#define puts(str) write(1, (str), strlen(str))

extern char bss;
extern char endOfBinary;

Data *data = NULL;

int children[MAX_PHYLOS + 1];

int make_printer() {
  int pid = fork();
  if (pid < 0) {
    puts("Failed to fork");
    return 1;
  } else if (pid == 0) {
    // child process
    print_state();
    exit(1);
  }
  children[MAX_PHYLOS] = pid;
  return 0;
}

int make_sem(int i) {
  data->phylos[i].state = THINKING;
  char sem_name[] = "sem_";
  rot_n(sem_name, i);
  sem_unlink(sem_name);
  data->phylos[i].sem = sem_open(sem_name, 1);
  if (data->phylos[i].sem == NULL) {
    puts("Failed to open semaphore\n");
    return 1;
  }
  return 0;
}

int make_philo(int i) {
  int pid = fork();
  if (pid < 0) {
    puts("Failed to fork");
    return 1;
  } else if (!pid) {
    // child process
    sem_wait(data->childex);
    phylosopher(i);
    exit(1);
  }
  // parent process
  children[i] = pid;
  (data->phylo_count)++;
  return 0;
}

/**
 * @brief Takes the right-most fork to avoid a philosopher having a fork that
 * iwll be moved, then adds a philospher to the right
 *
 * @param use_addex Whether to take the fork before adding a philosopher
 * @note If philosophers have been initialized, use_addex should always be true
 *
 * @return int 0 if successful, 1 otherwise
 */
int add_philo(int use_addex) {
  int i = data->phylo_count;

  if (i && use_addex) {
    sem_wait(data->mutex); // acquire mutex

    data->adding = i - 1;  // notify last philo you will add a new one
    sem_wait(data->addex); // wait for philo that must be notified to be in
                           // right state
  }
  data->phylos[i].state = THINKING; // All philos start in THINKING state
  if (make_sem(i) || make_philo(i)) {
    if (i && use_addex) {
      data->adding = -1;
      sem_post(data->mutex);
    }
    return 1;
  }

  if (i && use_addex) {
    data->adding = -1;
    sem_post(data->mutex);
    sem_wait(data->returnex);
  }

  return 0;
}

/**
 * @brief Takes the two right-most forks to avoid a philosopher having a fork
 * that will be removed/moved, then removes the right-most philosopher
 *
 */
void remove_phylo() {
  int i = data->phylo_count;
  sem_wait(data->mutex); // acquire mutex
  data->adding = i - 2;  // notify last philo that will remain that you will
                         // remove its neighbour
  sem_wait(
      data->addex); // wait for philo that must be notified to be in right state
  data->adding =
      i -
      1; // notify phylo that will be removed so it gives back its right fork
  sem_wait(data->addex);

  kill(children[i - 1], SIGKILL);
  waitpid(children[i - 1], NULL, 0);
  sem_close(data->phylos[i - 1].sem);
  (data->phylo_count)--;

  data->adding = -1;
  sem_post(data->mutex); // One of the processes blocked on this is dead, so I
                         // only need to post it once
  sem_wait(data->returnex);
}

int make_mutexes() {
  sem_unlink(
      "mutex"); // mutex para controlar el acceso a los estados de los filósofos
  sem_unlink("returnex"); // mutex para asegurar que se retornaron los tenedores
                          // antes de intentar sacar otro filósofo
  sem_unlink(
      "childex"); // mutex para controlar la inicialización de un nuevo filósofo
  sem_unlink("addex"); // mutex para controlar el acceso al último tenedor al
                       // añadir un filosofo
  sem_unlink("printex0");
  sem_unlink("printex1");

  data->mutex = sem_open("mutex", 1);
  data->returnex = sem_open("returnex", 0);
  data->childex = sem_open("childex", 0);
  data->addex = sem_open("addex", 0);
  data->printex[0] = sem_open("printex0", 1);
  data->printex[1] = sem_open("printex1", 0);

  if (data->mutex == NULL || data->childex == NULL || data->addex == NULL ||
      data->printex[0] == NULL || data->printex[1] == NULL ||
      data->returnex == NULL)
    return 1;
  return 0;
}

int main_loop() {
  char c;
  read(STD_IN, &c, 1);
  switch (c) {
  case 'a':
  case 'A': {
    if (data->phylo_count < MAX_PHYLOS) {
      puts("Adding\n");
      if (add_philo(1))
        return 1;
      sem_post(data->childex); // wake up the philo
      break;
    }
    puts("Limit reached\n");
    break;
  }
  case 'r':
  case 'R': {
    if (data->phylo_count <= 2) {
      puts("Can't have less than 2 phylos\n");
      break;
    }
    puts("Removing\n");
    remove_phylo();
    break;
  }
  case 'q':
  case 'Q': {
    puts("Quitting\n");
    return 1;
  }
  default:
    break;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  puts("Creating pseudo-shm\n");

  data = pshm(sizeof(Data)); // cumple la función de pseudo shm
  if (data == NULL) {
    puts("Failed to allocate memory\n");
    return 1;
  }
  data->printex = pshm(sizeof(sem_t *) * 2);
  if (data->printex == NULL) {
    free_shm(data);
    puts("Failed to allocate memory\n");
    return 1;
  }

  puts("Initializing mutexes\n");

  if (make_mutexes()) {
    puts("Failed to open semaphore\n");
    free_shm(data);
    return 1;
  }

  puts("Initializing philos\n");

  data->phylo_count = 0;
  data->adding = -1;
  setSeed(get_tick()); // This is questionable, but we aren't cryptographers

  for (unsigned int i = 0; i < INITIAL_PHYLOS; i++) {
    if (add_philo(0)) {
      leave();
      return 1;
    }
  }

  puts("Waking up philos\n");

  for (unsigned int i = 0; i < INITIAL_PHYLOS; i++) {
    sem_post(data->childex); // wake up the philos all at once
  }

  if (make_printer())
    return 1;

  while (!main_loop())
    ;
  leave();
  return 0;
}

void leave() {
  for (unsigned int i = 0; i < data->phylo_count; i++) {
    sem_close(data->phylos[i].sem);
    kill(children[i], SIGKILL);
    waitpid(children[i], NULL, 0);
  }
  kill(children[MAX_PHYLOS], SIGKILL);
  waitpid(children[MAX_PHYLOS], NULL, 0);
  sem_close(data->mutex);
  sem_close(data->returnex);
  sem_close(data->printex[0]);
  sem_close(data->printex[1]);
  free_shm(data->printex);
  free_shm(data);
  puts("byee!\n");
}
