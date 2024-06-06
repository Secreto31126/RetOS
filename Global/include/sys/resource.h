#ifndef GRSC_H
#define GRSC_H

#include <sys/types.h>

#define PRIO_PROCESS 0

/**
 * @brief Get the priority of a resource
 *
 * @param which The resource type of the `who` argument (PRIO_PROCESS (0) is the only valid value)
 * @param who The resource id
 * @return int The priority of the resource
 */
extern int getpriority(int which, id_t who);
/**
 * @brief Set the priority of a resource
 *
 * @param which The resource type of the `who` argument (PRIO_PROCESS (0) is the only valid value)
 * @param who The resource id
 * @param prio The new priority (ranged between -20 and 19, lower is higher priority)
 * @return int 0 on success, -1 on error
 */
extern int setpriority(int which, id_t who, int prio);
/**
 * @brief Open a pipe with the information of the process
 * @note You are responsible for closing the file descriptor
 *
 * @return int The file descriptor of the pipe, -1 on error
 */
extern int ps();

#endif
