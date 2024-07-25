#ifndef TSK_H
#define TSK_H

#include <stdint.h>

/**
 * @brief A task to execute
 * 
 * @param data The data to pass to the task
 * @return int 0 to finish, else reschedule
 */
typedef int (*Callback)(void *);

/**
 * @brief Patiently wait for a task to be executed
 */
void tasker_daemon();
/**
 * @brief Add a task to the scheduler
 *
 * @param task The task to execute
 * @param data The data to pass to the task
 * @return int 0 if success, -1 if error
 */
int add_task(Callback task, void *data);

#endif
