#ifndef GWAT_H
#define GWAT_H

#include <sys/types.h>

/**
 * @brief Block the current process until the child pid dies
 */
extern pid_t waitpid(pid_t pid, int *wstatus, int options);

#endif
