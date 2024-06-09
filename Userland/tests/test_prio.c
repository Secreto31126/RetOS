#include <stdint.h>
#include "syscall.h"
#include "test_util.h"

#define MINOR_WAIT "100000" // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 1000           // TODO: Change this value to make the wait long enough to see these processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 19  // TODO: Change as required
#define MEDIUM 10  // TODO: Change as required
#define HIGHEST -5 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio(uint64_t argc, char *argv[])
{
  // MODIFIED TO RECEIVE WAIT TIMES VIA ARGV, OR USE DEFAULTS IF NOT PRESENT
  uint64_t long_wait = WAIT, aux = 0;
  char *short_wait = MINOR_WAIT;
  if (argc >= 1 && (aux = satoi(argv[0])) > 0)
  {
    puts("\nUSING LONG WAIT FROM PARAMS\n");
    long_wait = aux;
  }
  if (argc >= 2 && satoi(argv[1]) > 0)
  {
    puts("\nUSING SHORT WAIT FROM PARAMS\n");
    short_wait = argv[1];
  }
  char *argvAux[] = {short_wait, NULL};
  // END OF MODIFICATION

  int pids[TOTAL_PROCESSES]; // MODIFIED TO ACCEPT NEGATIVE PIDS (I.E., my_create_process FAILED)
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
  {
    pids[i] = my_create_process("endless_loop_print", 1, argvAux);
    // MODIFIED TO NOTIFY ON FAILURE
    if (pids[i] < 0)
      puts("\nCREATE PROCESS FAILED\n");
    // END OF MODIFICATION
  }

  bussy_wait(long_wait);
  puts("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], prio[i]);

  bussy_wait(long_wait);
  puts("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
  {
    // MODIFIED TO PREVENT KILLING IDLE OR INIT ON A FATAL FAIL
    if (pids[i] <= 1)
    {
      puts("\nRECEIVED AN INVALID PID WHEN CREATING A PROCESS\n");
      goto killing;
    }
    // END OF MODIFICATION
    my_block(pids[i]);
  }

  puts("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], MEDIUM);

  puts("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);
  bussy_wait(long_wait);

killing:
  puts("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);
}
