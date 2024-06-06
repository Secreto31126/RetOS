#include <sys.h>
#include <signal.h>
#include <stdlib.h>
#include <semaphores.h>

#define MAX_ARGS 255

int64_t my_getpid()
{
  return get_pid();
}

// don't know how on earth you plan on making a pipe with this
// then again, I suppose you just don't
int64_t my_create_process(char *name, uint64_t argc, char *argv[])
{
  // All tests reside in the same module with a switch statement that uses argv[0] to select the test
  // For this reason, execv in this module has been routed to itself, selecting the test via argv

  char *modArgv[MAX_ARGS];
  int i = 0;
  modArgv[0] = name;
  for (i = 0; i < argc; i++)
    modArgv[i + 1] = argv[i];
  modArgv[i + 1] = NULL;

  int cPid = fork();
  if (cPid < 0)
  { // Failed to fork
    return -1;
  }
  if (!cPid)
  {
    // child

    // whoosh
    execv("tests", modArgv);

    // If execv fails, kill the child
    exit(1);
  }

  // parent
  return cPid;
}

int64_t my_nice(uint64_t pid, uint64_t newPrio)
{
  return 0;
}

// Added wait, since on being killed, processes can become zombies
int64_t my_kill(uint64_t pid)
{
  return kill(pid, SIGKILL) || waitpid(pid, NULL, 0);
}

int64_t my_block(uint64_t pid)
{
  return kill(pid, SIGSTOP);
}

int64_t my_unblock(uint64_t pid)
{
  return kill(pid, SIGCONT);
}

int64_t my_sem_open(char *sem_id, uint64_t initialValue)
{
  return 0;
}

int64_t my_sem_wait(char *sem_id)
{
  return 0;
}

int64_t my_sem_post(char *sem_id)
{
  return sem_post((sem_t *)sem_id);
}

int64_t my_sem_close(char *sem_id)
{
  return 0;
}

int64_t my_yield()
{
  yield();
  return 0;
}

int64_t my_wait(int64_t pid)
{
  return waitpid(pid, NULL, 0);
}
