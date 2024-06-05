#ifndef GSIG_H
#define GSIG_H

#include <sys/types.h>

#define SIGKILL 9
#define SIGCONT 18
#define SIGSTOP 19

/**
 * @brief Send a signal to a process
 *
 * @param pid The process' pid to send the signal
 * @param sig The signal to send (defined in Global signal.h)
 * @return int 0 on success, -1 on error
 */
extern int kill(pid_t pid, int sig);

#endif
