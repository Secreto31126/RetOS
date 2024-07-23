#ifndef MLCM_H
#define MLCM_H

typedef signed long int size_t_m;
// Do not edit
#define BLOCK_M 64
#define KB_129 132096
#define KB_1 1024
#define KB_128 131072
// This has been hardcoded, all processes are expected to have a heap of 129kb,
// with BLOCK of 64 bytes, and a resulting memory map of 128kb in 1kb
#define MAP_SIZE_M KB_1
#define MEM_SIZE_M KB_128
#define HEAP_SIZE_M KB_129

// Syscall to detect the start of the heap for the running process
void malloc_init();

typedef signed long int size_t_m;

void *malloc(size_t_m size);
void *realloc(void *ptr, size_t_m size);
void free(void *ptr);

// void print_m(char avoid_empty);
// void print_m_data();

#endif
