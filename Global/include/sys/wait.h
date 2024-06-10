#ifndef GWAT_H
#define GWAT_H

#include <sys/types.h>

/**
 * @brief Block the current process until the child pid dies
 *
 * @param pid The pid of the child process, -1 for any child
 * @param wstatus The exit status of the process
 * @param options The wait options
 * @return pid_t The pid of the process that exited, -1 if error
 */
extern pid_t waitpid(pid_t pid, int *wstatus, int options);

#endif
