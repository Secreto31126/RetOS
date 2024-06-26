#ifndef TST_H
#define TST_H
#include <stdint.h>

uint64_t test_sync(uint64_t argc, char *argv[]);
uint64_t test_mm(uint64_t argc, char *argv[]);
int64_t test_processes(uint64_t argc, char *argv[]);
void test_prio(uint64_t argc, char *argv[]);
uint64_t my_process_inc(uint64_t argc, char *argv[]);

#endif
