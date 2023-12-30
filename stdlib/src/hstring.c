#include "../include/hstring.h"

char* string_new(Allocator* allocator, size_t size) {
    char* new_str = allocator_alloc(allocator, size + 1);
    new_str[size] = '\0';
    return new_str;
}

char* string_from(Allocator* allocator, char* str) {
    char* new_str = allocator_alloc(allocator, strlen(str) + 1);
    if (new_str != NULL) {
        strcpy(new_str, str);
    }
    return new_str;
}

void string_delete(Allocator* allocator, char* str) {
    allocator_free(allocator, str);
}

char* string_resize(Allocator* allocator, char* str, size_t new_size) {
    char* new_str = allocator_alloc(allocator, new_size);
    if (new_str != NULL) {
        strcpy(new_str, str);
        allocator_free(allocator, str);
    } else {
        return str;
    }
    return new_str;
}

void string_clear(char* str) {
    memset(str, 0, strlen(str));
}

int string_compare(char* str1, char* str2) {
    if (strcmp(str1, str2) == 0) {
        return 1;
    }
    return 0;
}

void string_remove_new_lines(char* str) {
    char* i = str;
    char* j = str;
    
    while (*j != '\0') {
        *i = *j++;
        if (*i != '\n') {
            i++;
        }
    }
    *i = '\0';
}

int string_is_alphanumeric(char* str) {
    while (*str) {
        if (!isalnum(*str)) {
            return 0;
        }
        str++;
    }

    return 1;
}