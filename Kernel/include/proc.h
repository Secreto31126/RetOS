#ifndef PRC_H
#define PRC_H

#include <stddef.h>
#include <stdbool.h>
#include <pipes.h>
#include <semaphores.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX_PROCESS_FILES 10
#define MAX_PROCESSES 50

#define O_FILE 0x0
#define O_PIPE 0x8000

/* Priority limits.  */
#define PRIO_MIN -20 /* Minimum priority a process can have.  */
#define PRIO_MAX 19  /* Maximum priority a process can have.  */

/* The type of the WHICH argument to `getpriority' and `setpriority',
   indicating what flavor of entity the WHO argument specifies.  */
enum __priority_which
{
    PRIO_PROCESS = 0, /* WHO is a process ID.  */
#define PRIO_PROCESS PRIO_PROCESS
    PRIO_PGRP = 1, /* WHO is a process group ID.  */
#define PRIO_PGRP PRIO_PGRP
    PRIO_USER = 2 /* WHO is a user ID.  */
#define PRIO_USER PRIO_USER
};

/**
 * @brief An unique process identifier type
 * @note -1 indicates an invalid pid
 */
typedef int pid_t;
typedef unsigned int id_t;

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
 * @brief A conditional function to unblock a process
 *
 * @param pid The process' pid
 * @return bool True if the process should be unblocked, false otherwise
 */
typedef bool (*ProcessBlockConditional)(pid_t);

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
     * @brief The process' children head of the list
     */
    struct Process *next_child;
    /**
     * @brief The process' next brother
     */
    struct Process *next_brother;
    /**
     * @brief The process' exit code
     */
    int exit_code;
    /**
     * @brief The next process in the round-robin list
     *
     * @note The list head is always stored in the idle process (pid 0)
     */
    struct Process *next_robin;
    /**
     * @brief The process' priority (ranged between -20 and 19, lower is higher priority)
     */
    signed char priority;
    /**
     * @brief Points to the next blocked process
     *
     * @note The list head is always stored in the idle process (pid 0)
     */
    struct Process *next_blocked;
    /**
     * @brief The condition needed to be satisfied to unblock the process
     */
    ProcessBlockConditional block_condition;
    /**
     * @brief Any data required by the condition
     */
    void *condition_data[5];
    /**
     * @brief Process open file descriptors
     */
    int files[MAX_PROCESS_FILES];
} Process;

/**
 * @brief Create the first process (idle)
 *
 * @note Running stack will point to the same location as the stack,
 * so any reference to memory locations will be meaningless after
 * the first context switch. Kernel, don't keep pointers to the stack.
 *
 * @return void* The rsp of the new process
 */
void *create_process_idle();
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
 * @note This function should only be called from a syscall for "atomicity"
 *
 * @param pid The process' pid to kill
 * @return int 0 if the process was killed, error code otherwise
 */
int kill_process(pid_t pid);
/**
 * @brief Open a file in the current process
 * @note Kernel only, the syscall open() won't exist (yet)
 *
 * @param file The kernel file descriptor
 * @param flags O_ flags
 * @return int The process file descriptor, -1 if the file couldn't be opened
 */
int open_file(int file, int flags);
/**
 * @brief Close a file
 * @note Kernel only, use the syscall close() for closing current process' files
 *
 * @param file The process file descriptor
 * @return int 0 if the file was closed, -1 otherwise
 */
int close_file(int file);
/**
 * @brief Close a file in the current process
 *
 * @param fd The process file descriptor
 * @return int 0 if the file was closed, -1 otherwise
 */
int close(int fd);
/**
 * @brief Duplicate a file descriptor
 *
 * @param oldfd The old file descriptor to duplicate
 * @param newfd The new file descriptor to use
 * @return int
 */
int dup2(int oldfd, int newfd);
/**
 * @brief Get the priority of a resource
 *
 * @param which The resource type of the `who` argument (PRIO_PROCESS (0) is the only valid value)
 * @param who The resource id
 * @return int The priority of the resource
 */
int getpriority(int which, id_t who);
/**
 * @brief Set the priority of a resource
 *
 * @param which The resource type of the `who` argument (PRIO_PROCESS (0) is the only valid value)
 * @param who The resource id
 * @param prio The new priority (ranged between -20 and 19, lower is higher priority)
 * @return int 0 on success, -1 on error
 */
int setpriority(int which, id_t who, int prio);

/**
 * @brief Block the current process until the child pid dies
 */
pid_t waitpid(pid_t pid, int *wstatus, int options);
/**
 * @brief Block the current process for a number of ticks
 *
 * @param ticks The number of ticks to sleep
 * @return unsigned int The number of ticks left to sleep (always 0 in RetOS)
 */
unsigned int sleep(unsigned int ticks);
/**
 * @brief Block the current process for a number of microseconds (aka ticks)
 *
 * @param usec The number of microseconds to sleep
 * @return unsigned int 0 on success, -1 on error (never happens in RetOS)
 */
unsigned int usleep(unsigned int usec);
/**
 * @brief Add a read block to the current process
 *
 * @param file The file descriptor to read wait for
 */
void read_block(int file);
/**
 * @brief Add a write block to the current process
 *
 * @param file The file descriptor to write wait for
 */
void write_block(int file);
/**
 * @brief Primitive select implementation
 *
 * @param nfds The number of file descriptors to check
 * @param fds The file descriptors to check
 * @param ready An array to store the ready file descriptors
 * @return int The number of ready file descriptors, 0 if all waited files are closed
 */
int pselect(int nfds, const int *fds, int *ready);
/**
 * @brief Add a semaphore block to the current process
 *
 * @param sem The semaphore to wait for
 */
void sem_block(sem_t *sem);

/**
 * @brief Send a signal to a process
 *
 * @param pid The process' pid to send the signal
 * @param sig The signal to send (defined in Global signal.h)
 * @return int 0 on success, -1 on error
 */
int kill(pid_t pid, int sig);

/**
 * @brief Skip remaining CPU time and give it to the next process
 */
extern void yield();
/**
 * @brief Kill the current process and halt
 * @note This function should only be called from a syscall for "atomicity"
 */
extern void exit(int status);

#endif
