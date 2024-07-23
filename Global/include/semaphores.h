#ifndef GSEM_H
#define GSEM_H

typedef struct {
  char name[32];
  unsigned int value;
} sem_t;

/**
 * @brief Initializes a named semaphore, if it doesn't exist
 *       creates it with the given value
 *
 * @param name Name of the semaphore
 * @param value Initial value of the semaphore
 *
 * @return 0 on success, -1 on error
 */
extern sem_t *sem_open(const char *name, unsigned int value);

/**
 * @brief Closes the semaphore
 *
 * @param sem Semaphore to close
 *
 * @return 0 on success, -1 on error
 */
extern int sem_close(sem_t *sem);

/**
 * @brief Closes the semaphore
 *
 * @param sem Semaphore to close
 *
 * @return 0 on success, -1 on error
 */
extern int sem_unlink(const char *name);

/**
 * @brief Increments the semaphore value
 *
 * @param sem Semaphore to post
 *
 * @return 0 on success, -1 on error
 */
extern int sem_post(sem_t *sem);

/**
 * @brief Waits for the semaphore to be available and decrements its value
 *
 * @param sem Semaphore to wait for
 *
 * @return 0 on success, -1 on error
 */
extern int sem_wait(sem_t *sem);

/**
 * @brief Retrieves the value of the semaphore
 *
 * @param sem The semaphore to get the value from
 * @param sval The direction to store the value
 * @return int 0 on success, -1 on error
 */
extern int sem_getvalue(sem_t *sem, int *sval);

/**
 * @brief Initializes an anonymous semaphore
 *
 * @param sem Where to store the semaphore
 * @param pshared Wether or not the semaphore is shared between processes
 * @param value The initial value of the semaphore
 * @return int 0 on success, -1 on error
 */
extern int sem_init(sem_t *sem, int pshared, unsigned int value);

/**
 * @brief Destroys the semaphore
 *
 * @param sem The semaphore to destroy
 * @return int 0 on success, -1 on error
 */
extern int sem_destroy(sem_t *sem);

#endif
