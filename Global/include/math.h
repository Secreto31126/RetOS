#ifndef GMTH_H
#define GMTH_H

#define PI 3.14159265359

/**
 * @brief Exponential function
 *
 * @note can't return doubles in userland because SSE registers aren't usable
 *
 * @param x Base
 * @param y Exponent
 * @return unsigned long long Result
 */
unsigned long long pow(double x, int y);

#endif
