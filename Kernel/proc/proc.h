#ifndef PPRC_H
#define PPRC_H

#include <lib.h>
#include <proc.h>
#include <memory.h>
#include <console.h>

#define MIN_STACK_SIZE 0x80000
#define INIT_STACK_SIZE 0x400

#define STACK_END(stack, size) (((void *)(stack)) + (size))

/**
 * @brief The amount of active processes
 */
extern pid_t processes_count;

/**
 * @brief Set the pid value
 *
 * @param p The new pid value
 * @return pid_t The previous pid value
 */
pid_t set_pid(pid_t p);

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
