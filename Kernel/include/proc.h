#ifndef PRC_H
#define PRC_H

#include <stddef.h>

#define MAX_PROCESS_CHILDREN 2
#define MAX_PROCESSES 10

/**
 * @brief An unique process identifier type
 * @note -1 indicates an invalid pid
 */
typedef int pid_t;

/**
 * @brief Enum of all the possible process' states
 */
typedef enum ProcessStateEnum
{
    /**
     * @brief Empty state
     */
    NOT_THE_PROCESS_YOU_ARE_LOOKING_FOR = '\0',
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
    /**
     * @brief Caput
     */
    PROCESS_DEAD = 'D',
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
     * @brief The parent process id
     */
    pid_t ppid;
    /**
     * @brief The process allocated stack
     */
    void *stack;
    /**
     * @brief Where the stack should be placed to run the process, inherited from the parent
     */
    void *running_stack;
    /**
     * @brief The process' stack size, in bytes
     */
    size_t stack_size;
    /**
     * @brief The process' running stack size, in bytes, inherited from the parent
     */
    size_t running_stack_size;
    /**
     * @brief The process rsp
     */
    void *rsp;
    /**
     * @brief The process state, configured by the scheduler in context_switch()
     */
    ProcessState state;
    /**
     * @brief The process' children
     * @note MAX_PROCESSES because it might inherit all the children from a child
     */
    pid_t children[MAX_PROCESSES];
    /**
     * @brief The process' children count
     */
    size_t children_count;
} Process;

/**
 * @brief Create the init process
 *
 * @note Running stack will point to the same location as the stack,
 * so any reference to memory locations will be meaningless after
 * the first context switch. Kernel, don't keep pointers to the stack.
 *
 * @return void* The rsp of the new process
 */
void *create_process_init();
/**
 * @brief Get the current process
 *
 * @return Process* The current process
 */
Process *get_current_process();
/**
 * @brief Get a process by its pid
 *
 * @param pid The process' pid to get
 * @return Process* The process
 */
Process *get_process(pid_t pid);
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
 * Looking at you, kernel. Don't use it.
 *
 * @param rsp The rsp with the context switch header
 * @return pid_t The new process' pid
 */
pid_t create_process(void *rsp);
/**
 * @brief Kill a process by its pid
 *
 * @param pid The process' pid to kill
 * @return int 0 if the process was killed, error code otherwise
 */
int kill_process(pid_t pid);

/**
 * @brief Kill the current process and halt
 */
extern void exit();

#endif
