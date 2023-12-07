#include "../include/allocator.h"

Allocator global_allocator = { NULL, 0, 0 };
/*
TODO: Use linked lists in the final version
*/

void allocator_initialize(Allocator* allocator, size_t capacity) {
    if (capacity <= 0) // Shouldn't be smaller than zero since its unsigned. But better to be safe :)
        capacity = 2;
    allocator->allocations = malloc(capacity * sizeof(void*));
    allocator->capacity = capacity;
    allocator->current_size = 0;
}

void allocator_cleanup(Allocator* allocator) {
    for (int i = 0; i < allocator->capacity; i++) {
        if (allocator->allocations[i] != 0 && allocator->allocations[i] != NULL) {
#if ALLOCATOR_INFORMATIONS == 1
            printf("Free'd %p\n", allocator->allocations[i]);
#endif
            free(allocator->allocations[i]);
            allocator->allocations[i] = 0;
        }
    }
    free(allocator->allocations);
}

void* allocator_alloc(Allocator* allocator, size_t size) {
    void* allocation = malloc(size);
#if ALLOCATOR_INFORMATIONS == 1
    printf("Allocated new memory at: %p\n", allocation);
#endif
    if (allocator->current_size == allocator->capacity) {
        allocator->capacity += 5;
        allocator->allocations = realloc(allocator->allocations, allocator->capacity * sizeof(void*));
    }
    allocator->allocations[allocator->current_size] = allocation;
    allocator->current_size++;
    return allocation;
}

void allocator_free(Allocator* allocator, void* ptr) {
    for (int i = 0; i < allocator->capacity; i++) {
        if (allocator->allocations[i] == ptr) {
            if (allocator->allocations[i] != 0 && allocator->allocations[i] != NULL) {
#if ALLOCATOR_INFORMATIONS == 1
            printf("Free'd %p\n", allocator->allocations[i]);
#endif
                free(allocator->allocations[i]);
                allocator->allocations[i] = 0;
                break;
            }
        }
    }

#if ALLOCATOR_OPTIMIZED_FREE == 1
    int unused_slots = 0;
    int current_size = 0;
    for (int i = 0; i < allocator->capacity; i++) {
        if (allocator->allocations[i] == 0 || allocator->allocations[i] == NULL) {
            unused_slots++;
        }
    }

    size_t new_capacity = allocator->capacity / 2;
    if (unused_slots > new_capacity) {
        new_capacity += 1;
        if (new_capacity <= 0)
            new_capacity = 10;
        void** new_allocation = malloc(new_capacity * sizeof(void*));
#if ALLOCATOR_INFORMATIONS == 1
        printf("Allocated new array at: %p \nCapacity of array: %lu\n", new_allocation, new_capacity);
#endif
        for (int i = 0; i < allocator->capacity; i++) {
             if (allocator->allocations[i] != 0 && allocator->allocations[i] != NULL) {
                new_allocation[current_size] = allocator->allocations[i];
                current_size++; 
             }
        }
        allocator->current_size = current_size;
        allocator->capacity = new_capacity;
        free(allocator->allocations);
        allocator->allocations = new_allocation;
    }
#endif
}

void allocator_print_information(Allocator* allocator) {
    for (int i = 0; i < global_allocator.capacity; i++) {
        printf("%p\n", global_allocator.allocations[i]);
    }
}