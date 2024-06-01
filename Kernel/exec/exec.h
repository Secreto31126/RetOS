#ifndef PEXE_H
#define PEXE_H

#include <mman.h>
#include <proc.h>
#include <ticks.h>
#include <string.h>
#include <stdint.h>
#include <memory.h>

#define EXECUTABLES 7
#define MAX_ARGS 256
/**
 * @brief Each argument max length
 * @note Includes the NULL terminator
 */
#define MAX_ARG_LEN 1024

/**
 * @brief Userland entry point
 */
typedef void (*EntryPoint)(void);

/**
 * @brief Executable file
 */
typedef struct
{
    /**
     * @brief Executable filename
     */
    char *filename;
    /**
     * @brief Execute Permission (Bit wise RWX)
     */
    int mod;
    /**
     * @brief Executable code
     */
    EntryPoint main;
} Executable;

/**
 * @brief Small type to hold the rsp pointer
 */
typedef void **RSP;
/**
 * @brief Small type to hold the top of the stack pointer
 */
typedef char *Stack;

/**
 * @brief Array of all of RetOS executables
 */
extern Executable executables[EXECUTABLES];

/**
 * @brief Validate the arguments and count them (not including the NULL terminator)
 *
 * @param argv The argument values, NULL terminated, it may be NULL and return 0
 * @return int argc if success, negative if error
 */
int check_args(char *const argv[]);
/**
 * @brief Set the process' new arguments
 * @note Resets the process' RSP to the bottom of the stack and pushes the arguments there
 *
 * @param argc The number of arguments
 * @param argv The argument values, NULL terminated
 * @param process The process to set the arguments to
 * @return RSP The new rsp pointer
 */
RSP set_process_args(int argc, char const argv[MAX_ARGS][MAX_ARG_LEN], Process *process);
/**
 * @brief Save the arguments somewhere, as long as it's not in the constantly moving stack
 *
 * @param argc The number of arguments (asumes it was validated with check_args())
 * @param argv The argument values, NULL terminated
 * @return char* const* const The backup pointer
 */
char *const *const backup_argv_somewhere(int argc, char *const argv[]);

extern void portal_to_userland(int argc, char *const argv[], Process *process, EntryPoint code);

#endif
