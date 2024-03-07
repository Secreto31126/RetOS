#ifndef EXE_H
#define EXE_H

/**
 * @brief Starts an executable file overwriting the current process
 *
 * @param pathname The executable's pathname
 * @param argv The arguments to start the executable with, it must be null terminated, it may be null
 * @return int error code, does not return if success
 */
int execv(const char *pathname, char *const argv[]);

#endif
