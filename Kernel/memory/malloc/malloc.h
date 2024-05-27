#ifndef MLC_H
#define MLC_H

// Set the minimum size of a memory block
#define BLOCK 256

typedef signed long int size_t_m;

void set_heap(void *start, size_t_m size);

void *malloc_m(size_t_m size);
void *realloc_m(void *ptr, size_t_m size);
void free_m(void *ptr);
// void print_m(char avoid_empty);
// void print_m_data();

#endif
