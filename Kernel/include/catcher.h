#ifndef CTH_H
#define CTH_H

#include <stdint.h>

/**
 * @brief This function is called when an exception is thrown
 *
 * @param exception The exception number
 * @param registers The register's dump
 */
typedef void (*Catch)(uint64_t exception, char *registers);

extern void handle_catch(uint64t exception);
/**
 * @brief Setup the catch for a CPU exception
 *
 * @param c The function to call on exception
 * @param rbp The stack restoration pointer
 */
extern void setup_catch(Catch c, uint64_t rbp);

#endif
