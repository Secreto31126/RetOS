#ifndef PRC_H
#define PRC_H

/**
 * @brief An unique process identifier type
 */
typedef int pid_t;

/**
 * @brief Enum of all the possible process' states
 */
typedef enum ProcessStateEnum
{
    /**
     * @brief Running
     */
    PROCESS_RUNNING = 'R',
    /**
     * @brief Ready/Waiting
     */
    PROCESS_READY = 'W',
    /**
     * @brief Blocked
     * TODO: Implement this state
     */
    PROCESS_BLOCKED = 'B',
    /**
     * @brief Zombie
     * TODO: Implement this state
     */
    PROCESS_ZOMBIE = 'Z',
} ProcessState;

/**
 * @brief The Process structure
 */
typedef struct Process
{
    /**
     * @brief The process id
     */
    pid_t pid;
    /**
     * @brief The process allocated stack
     */
    void *stack;
    /**
     * @brief The process' stack size
     */
    size_t stack_size;
    /**
     * @brief The process rsp
     */
    void *rsp;
    /**
     * @brief The process state, configured by the scheduler in context_switch()
     */
    ProcessState state;
} Process;

/**
 * @brief Get the current process
 *
 * @return Process* The current process
 */
Process *get_current_process();
/**
 * @brief Get the currently executing pid
 *
 * @return pid_t The current pid
 */
pid_t get_pid();
/**
 * @brief Create a new process
 *
 * @note This function must ALWAYS be called from a interruption context.
 * Looking at you, kernel. Use kernel_fork() instead.
 *
 * @param rsp The rsp with the context switch header
 * @param i_rsp The interruption rsp direction
 * @return pid_t The new process' pid
 */
pid_t create_process(void *rsp, void **i_rsp);

extern void *kernel_fork();

#endif
