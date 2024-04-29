#ifndef PPRC_H
#define PPRC_H

#include <lib.h>
#include <proc.h>
#include <memory.h>

#define MAX_PROCESSES 10
#define MIN_STACK_SIZE 0x8000

/**
 * @brief The current process ID
 */
extern pid_t pid;

/**
 * @brief The amount of active processes (processes' length)
 */
extern pid_t processes_count;

/**
 * @brief The array of all processes
 * @note In the future, this will be a Tree (or not, who knows?)
 */
extern Process processes[MAX_PROCESSES];

extern void *get_rsp();

#endif
