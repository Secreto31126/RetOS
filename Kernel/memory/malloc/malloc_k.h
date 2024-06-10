#ifndef MLCK_H
#define MLCK_H

// Set the minimum size of a memory block
#define BLOCK 256

typedef signed long int size_t_k;

void malloc_init(void *start, size_t_k size);

void *malloc_m(size_t_k size);
void *realloc_m(void *ptr, size_t_k size);
void free_m(void *ptr);
// void print_m(char avoid_empty);
// void print_m_data();

#endif
