#include "syscall.h"
#include "test_util.h"
#define MAX_PROCS 32 // This isn't the OS's max process capacity
// However, managing more than this amount of processes in this test will almost
// certainly affect performance enough to make reading its results next to
// impossible

enum State { RUNNING, BLOCKED, KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

int64_t test_processes(uint64_t argc, char *argv[]) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {0};

  if (argc != 1)
    return -2;
  if ((max_processes = satoi(argv[0])) <= 0)
    return -2;

  // MODIFIED TO AVOID INVALID PARAMETERS
  if (max_processes > MAX_PROCS) {
    puts("REDUCED NUMBER OF PROCESSES REQUESTED TO 32");
    max_processes = MAX_PROCS;
  }
  // END OF MODIFICATION

  p_rq p_rqs[MAX_PROCS];

  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = my_create_process("endless_loop", 0, argvAux);

      if (p_rqs[rq].pid == -1) {
        puts("test_processes: ERROR creating process\n");
        return -1;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been
    // killed
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = get_tick() % 2;

        switch (action) {
        case 0:
          if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
            if (my_kill(p_rqs[rq].pid) == -1) {
              puts("test_processes: ERROR killing process\n");
              return -1;
            }
            my_wait(p_rqs[rq].pid); // MODIFIED ADDED A WAIT TO PREVENT
                                    // ACCUMULATING ZOMBIE PROCESSES
            p_rqs[rq].state = KILLED;
            alive--;
          }
          break;

        case 1:
          if (p_rqs[rq].state == RUNNING) {
            if (my_block(p_rqs[rq].pid) == -1) {
              puts("test_processes: ERROR blocking process\n");
              return -1;
            }
            p_rqs[rq].state = BLOCKED;
          }
          break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && get_tick() % 2) {
          if (my_unblock(p_rqs[rq].pid) == -1) {
            puts("test_processes: ERROR unblocking process\n");
            return -1;
          }
          p_rqs[rq].state = RUNNING;
        }
    }
  }
}
