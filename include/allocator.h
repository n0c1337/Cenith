#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>
#include <stdio.h>

/*
0 = disabled
1 = enabled
*/
#define ALLOCATOR_OPTIMIZED_FREE 1 
#define ALLOCATOR_INFORMATIONS 0

typedef struct allocator_t {
    void** allocations;
    size_t capacity;
    int current_size;
} Allocator;

extern Allocator global_allocator;

void allocator_initialize(Allocator* allocator, size_t capacity);
void allocator_cleanup(Allocator* allocator);
void* allocator_alloc(Allocator* allocator, size_t size);
void allocator_free(Allocator* allocator, void* ptr);
void allocator_print_information(Allocator* allocator);

#endif