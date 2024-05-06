#ifndef PPRC_H
#define PPRC_H

#include <lib.h>
#include <proc.h>
#include <memory.h>
#include <console.h>
#include <stdbool.h>

#define MIN_STACK_SIZE 0x80000
#define INIT_STACK_SIZE 0x400

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
 * @brief Let a process inherit its parent's stack
 * @note Blocks any process with equal running_stack from running (TODO: Use a semaphore for this)
 * @note This function should only be called from kill_pid
 *
 * @param process The process to inherit its parent's house
 * @return true if the process inherited its parent's house, false otherwise
 */
bool inherit_parents_house(Process *process);

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
