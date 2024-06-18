#ifndef GPSL_H
#define GPSL_H

/**
 * @brief Primitive select implementation
 *
 * @param nfds The number of file descriptors to check
 * @param fds The file descriptors to check
 * @param ready An array to store the ready file descriptors
 * @return int The number of ready file descriptors, 0 if all waited files are
 * closed, -1 on error
 */
extern int pselect(int nfds, const int *fds, int *ready);

#endif
