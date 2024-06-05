#include <sys.h>
#include <signal.h>
#include <stdlib.h>

int64_t my_getpid()
{
  return get_pid();
}

// don't know how on earth you plan on making a pipe with this
// then again, I suppose you just don't
int64_t my_create_process(char *name, uint64_t argc, char *argv[])
{
  int cPid = fork();
  if (cPid < 0)
  { // Failed to fork
    return -1;
  }
  if (!cPid)
  {
    // child
    execv(name, argv);
  }
  return cPid;
}

int64_t my_nice(uint64_t pid, uint64_t newPrio)
{
  return 0;
}

int64_t my_kill(uint64_t pid)
{
  return kill(pid, SIGKILL);
}

int64_t my_block(uint64_t pid)
{
  return 0;
}

int64_t my_unblock(uint64_t pid)
{
  return 0;
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
  return 0;
}

int64_t my_sem_close(char *sem_id)
{
  return 0;
}

int64_t my_yield()
{
  return 0;
}

int64_t my_wait(int64_t pid)
{
  return 0;
}
