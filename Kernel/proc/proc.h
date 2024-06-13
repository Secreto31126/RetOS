#ifndef PPRC_H
#define PPRC_H

#include <lib.h>
#include <proc.h>
#include <mman.h>
#include <files.h>
#include <ticks.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <signal.h>
#include <console.h>
#include <stdbool.h>
#include <semaphores.h>

#define MIN_STACK_SIZE 0x80000
#define IDLE_STACK_SIZE 0x400

#define STACK_END(stack, size) (((void *)(stack)) + (size))

/**
 * @brief The amount of active processes
 */
extern pid_t active_processes_count;

/**
 * @brief Set the pid value
 *
 * @param p The new pid value
 * @return pid_t The previous pid value
 */
pid_t set_pid(pid_t p);
/**
 * @brief Set the exit code to the current process
 *
 * @note Should only be used by _exit()
 *
 * @param status The exit code
 */
void set_exit_code(int status);
/**
 * @brief Let a process inherit its parent's stack
 * @note This function should only be called from kill_pid
 * (which should only be called from a syscall for "atomicity")
 *
 * @param process The process to inherit its parent's house
 * @return true if the process inherited its parent's house, false otherwise
 */
bool inherit_parents_house(Process *process);

/**
 * @brief Wait for multiple fds to be read available
 * @note The process' pointers will be invalid while checking because the running stack is not loaded
 * Aka, you must allocate global memory to preserve the data
 *
 * @param pid The process' pid
 * @return true At least one file is available for reading or all files are closed
 * @return false No file is available for reading
 */
bool multi_read_available(pid_t pid);
/**
 * @brief Await for a sleep to finish
 *
 * @param pid The process' pid
 * @return true Condition met
 * @return false Condition not met
 */
bool sleep_finished(pid_t pid);
/**
 * @brief No condition required
 *
 * @param pid The process' pid
 * @return true Condition always met
 */
bool no_condition(pid_t pid);
/**
 * @brief Keep the process blocked forever
 *
 * @param pid The process' pid
 * @return false Condition never met
 */
bool whim_condition(pid_t pid);

/**
 * @brief Loop all the blocked processes and unblock them if the condition is met
 */
void check_tick_blocked_processes();
/**
 * @brief Add a process to the blocked queue
 * @note If the process is already blocked, it only updates the condition and data
 *
 * @note It could be a vaargs
 * @note ¯\_(ツ)_/¯
 *
 * @param p The process to block
 * @param condition The condition to unblock the process
 * @param data0 Condition data
 * @param data1 Condition data
 * @param data2 Condition data
 * @param data3 Condition data
 * @param data4 Condition data
 */
void add_tick_blocked(Process *p, ProcessBlockConditional condition, void *data0, void *data1, void *data2, void *data3, void *data4);

/**
 * @brief Add a process to the round robin queue
 * @note This function should only be called from create_process
 *
 * @param pid The process' pid
 */
void robin_add(pid_t pid);
/**
 * @brief Get the next process in the round robin queue
 *
 * @return pid_t The next process' pid, 0 if no process is available
 */
pid_t robin_next();
/**
 * @brief Remove a process from the round robin queue
 * @note If a process dies, it's not necessary to remove it from the queue,
 * calling robin_next will check its state and delete it if necessary
 *
 * @param pid The process' pid
 */
pid_t robin_remove(pid_t pid);
/**
 * @brief Set the remainin ticks to 0
 * @note This function should only be called from sched_yield()
 */
void yield_robin();

/**
 * @brief Swaps the contents of 2 stacks
 * @note No change if both stacks are equal
 * @note RSP should NEVER be pointing to either stack, for obvious reasons
 *
 * @cite "I want pagination!"
 * "We have pagination at home"
 * Pagination at home:
 *
 * @param current Stack 1
 * @param stash Stack 2
 * @param size The size of the stacks to swap
 */
extern void swap_stacks(void *current, void *stash, size_t size);

#endif
