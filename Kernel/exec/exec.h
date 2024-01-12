#ifndef PEXE_H
#define PEXE_H

#include <lib.h>
#include <ticks.h>
#include <stdint.h>

#define EXECUTABLES 2

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
 * @brief Array of all of RetOS executables
 */
extern Executable executables[EXECUTABLES];

void *getStackBase();

extern int portal_to_userland(int argc, char *const argv[], EntryPoint code, void *rsp);

#endif
