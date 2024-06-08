#include <stdint.h>
#include "syscall.h"
#include "test_util.h"

#define MINOR_WAIT "10000" // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 19  // TODO: Change as required
#define MEDIUM 0   // TODO: Change as required
#define HIGHEST -5 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio(uint64_t argc, char *argv[])
{
  // MODIFIED TO RECEIVE WAIT TIMES VIA ARGV
  uint64_t long_wait = WAIT, short_wait = MINOR_WAIT, aux = 0;
  if (argc >= 1 && (aux = satoi(argv[0])) > 0)
  {
    long_wait = aux;
  }
  if (argc >= 2 && satoi(argv[1]) > 0)
  {
    short_wait = argv[1];
  }
  char *argvAux[] = {short_wait, NULL};
  // END OF MODIFICATION

  int64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = my_create_process("endless_loop_print", 1, argvAux);

  bussy_wait(long_wait);
  puts("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], prio[i]);

  bussy_wait(long_wait);
  puts("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_block(pids[i]);

  puts("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], MEDIUM);

  puts("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);

  bussy_wait(long_wait);
  puts("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);
}
