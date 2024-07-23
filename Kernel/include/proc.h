#ifndef PRC_H
#define PRC_H

#include <pipes.h>
#include <semaphores.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

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
enum __priority_which {
  PRIO_PROCESS = 0, /* WHO is a process ID.  */
#define PRIO_PROCESS PRIO_PROCESS
  PRIO_PGRP = 1, /* WHO is a process group ID.  */
#define PRIO_PGRP PRIO_PGRP
  PRIO_USER = 2 /* WHO is a user ID.  */
#define PRIO_USER PRIO_USER
};

/**
 * @brief Enum of all the possible process' states
 */
typedef enum ProcessStateEnum {
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
typedef struct Process {
  /**
   * @brief The process id
   */
  pid_t pid;
  /**
   * @brief The parent process id
   */
  pid_t ppid;
  /**
   * @brief The process name
   */
  char *name;
  /**
   * @brief The process allocated stack
   */
  void *stack;
  /**
   * @brief Where the stack should be placed to run the process, inherited from
   * the parent
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
   * @brief The process' priority (ranged between -20 and 19, lower is higher
   * priority)
   */
  signed char priority;
  /**
   * @brief The list head that holds the process blocked
   */
  struct Process **block_list;
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
   * @brief The children's death semaphore
   */
  sem_t zombie_sem;
  /**
   * @brief The process' death semaphore
   */
  sem_t exit_sem;
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
 * @brief Iterate over a blocked process list and unblock the ones that satisfy
 * the condition
 *
 * @param p The head of the blocked process list
 * @return Process* The new head of the blocked process list
 */
Process *loop_blocked_and_unblock(Process *p);

#endif
