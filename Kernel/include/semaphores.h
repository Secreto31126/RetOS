#ifndef SEM_H
#define SEM_H

#include <stddef.h>
#include <stdbool.h>
#include <proc.h>

typedef struct sem_t
{
    char name[255];
    unsigned int value;
} sem_t;

/**
 * @brief Initializes an anonymous semaphore
 *
 * @param sem where to store the semaphore
 * @param value Initial value of the semaphore
 *
 * @return 0 on success, -1 on error
 */
int init_sem(sem_t *sem, unsigned int value);

/**
 * @brief Initializes a named semaphore, if it doesn't exist
 *       creates it with the given value
 *
 * @param name Name of the semaphore
 * @param value Initial value of the semaphore
 *
 * @return 0 on success, -1 on error
 */
sem_t *sem_open(const char *name, unsigned int value);

/**
 * @brief Waits for the semaphore to be available and decrements its value
 *
 * @param sem Semaphore to wait for
 *
 * @return 0 on success, -1 on error
 */
int sem_wait(sem_t *sem);

/**
 * @brief Increments the semaphore value
 *
 * @param sem Semaphore to post
 *
 * @return 0 on success, -1 on error
 */
int sem_post(sem_t *sem);

/**
 * @brief Closes the semaphore
 *
 * @param sem Semaphore to close
 *
 * @return 0 on success, -1 on error
 */
int sem_unlink(const char *name);

/**
 * @brief Closes the semaphore
 *
 * @param sem Semaphore to close
 *
 * @return 0 on success, -1 on error
 */
int close_sem(sem_t *sem);

#endif