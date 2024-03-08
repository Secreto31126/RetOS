#ifndef PEXE_H
#define PEXE_H

#include <lib.h>
#include <ticks.h>
#include <stdint.h>
#include <memory.h>

#define EXECUTABLES 2
#define MAX_ARGS 255
#define STACK_SIZE 0x8000

/**
 * @brief Userland entry point
 */
typedef int (*EntryPoint)();

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
    uint64_t mod;
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
typedef char **Stack;

extern int pid;
/**
 * @brief Array of all of RetOS executables
 */
extern Executable executables[EXECUTABLES];

/**
 * @brief Allocate stack memory and return the top and base of the stack
 *
 * @param rsp The base of the stack (top + size - sizeof(uint64_t))
 * @return void* The top of the stack, NULL if error
 */
Stack getStackTopAndBase(RSP *rsp);
/**
 * @brief Set the stack's argv
 *
 * @param rsp The stack's base
 * @param argv The argument values
 * @return int argc if success, negative if error
 */
int setStackArgs(RSP *rsp, char *const argv[], Executable executable);
/**
 * @brief Free the stack memory
 *
 * @param stack The stack memory
 * @param rsp The stack base
 * @param argc The number of arguments
 */
void freeStack(Stack stack, RSP rsp, int argc);

extern void portal_to_userland(EntryPoint code, RSP rsp);

#endif
