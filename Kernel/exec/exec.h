#ifndef PEXE_H
#define PEXE_H

#include <lib.h>
#include <proc.h>
#include <ticks.h>
#include <stdint.h>
#include <memory.h>

#define EXECUTABLES 3
#define MAX_ARGS 255

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
 * @brief Validate the arguments and count them
 *
 * @param argv The argument values, NULL terminated, it may be NULL and return 0
 * @return int argc if success, negative if error
 */
int check_args(char *const argv[]);
/**
 * @brief Set the process' stack arguments
 *
 * @param argc The number of arguments
 * @param argv The argument values, NULL terminated
 * @param heap The heap memory of the process, the argv strings will be copied here
 * (Hippity hoppity, your heap is now my property :] )
 * @param rsp The rsp pointer of the process
 */
void set_stack_args(int argc, char *const argv[], Stack heap, RSP *rsp);

extern void portal_to_userland(EntryPoint code, RSP rsp);

#endif
