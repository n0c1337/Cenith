#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>

#define STD_ALLOCATOR_ENHANCED_INFORMATIONS 0

#if STD_ALLOCATOR_ENHANCED_INFORMATIONS == 1
#include <stdio.h>
#endif

typedef struct linkedlist_t {
    void* value;
    struct linkedlist_t* next;
} LinkedList;

typedef struct allocator_t {
    LinkedList* allocations;
} Allocator;


extern Allocator global_allocator;

void allocator_initialize(Allocator* allocator);
void allocator_destroy(Allocator* allocator);
void* allocator_alloc(Allocator* allocator, size_t size);
void allocator_free(Allocator* allocator, void* ptr);

#endif