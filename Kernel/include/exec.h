#ifndef EXE_H
#define EXE_H

/**
 * @brief Starts an executable file
 *
 * @param pathname The executable's pathname
 * @param argv The arguments to start the executable with
 * @return int 0 on success, positives values are executable errors and negative execv errors
 */
int execv(const char *pathname, char *const argv[]);

#endif
