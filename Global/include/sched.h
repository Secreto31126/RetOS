#ifndef GSCH_H
#define GSCH_H

/**
 * @brief Skip remaining CPU time and give it to the next process
 * @note If no other process is available, the current process will continue
 *
 * @return int 0 on success, -1 on failure (never fails)
 */
extern int sched_yield();

#endif
