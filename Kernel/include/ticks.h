#ifndef TKS_H
#define TKS_H

#include <stdint.h>

typedef void (*Callback)(void);

/**
 * @brief Get the tick value
 * @note 18 tick = 1s, 1 tick = 55.555ms
 *
 * @return uint64_t tick value
 */
uint64_t get_tick();
/**
 * @brief Update the tick value
 */
void update_tick();
/**
 * @brief Sleep for a certain amount of ticks
 *
 * @param ticks Amount of ticks to sleep
 */
void sleep_ticks(uint64_t ticks);
/**
 * @brief Add a task to the scheduler
 *
 * @param ticks Amount of ticks to wait before executing the task
 * @param task The task to execute
 */
void add_task(uint64_t ticks, Callback task);

#endif
