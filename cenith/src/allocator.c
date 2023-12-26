#include "../include/allocator.h"

Allocator global_allocator = { NULL, 0, 0 };

void allocator_initialize(Allocator* allocator) {
    allocator->allocations = malloc(sizeof(LinkedList));
    allocator->allocations->value = NULL;
    allocator->allocations->next = NULL;
}

void allocator_destroy(Allocator* allocator) {
    LinkedList* current = allocator->allocations;
    LinkedList* next = NULL;

    while (current != NULL) {
        next = current->next;

        if (current->value != NULL) {
#if STD_ALLOCATOR_ENHANCED_INFORMATIONS == 1
            printf("Free'd %p\n", current->value);
#endif
            free(current->value);
            current->value = NULL;
        }
        free(current);
        current = next;
    }
}

void* allocator_alloc(Allocator* allocator, size_t size) {
    void* allocation = malloc(size);

    LinkedList* new_node = malloc(sizeof(LinkedList));
    new_node->value = allocation;
    new_node->next = NULL;

    LinkedList* current = allocator->allocations;

    while (current->next != NULL) {
        current = current->next;
    }
    
    current->next = new_node;

    return allocation;
}

void allocator_free(Allocator* allocator, void* ptr) {
    LinkedList* current = allocator->allocations;

    while (current->value != ptr || current != NULL) {
        current = current->next;
    }

    if (current->value == ptr) {
        free(ptr);
        current->value = NULL;
    }
}