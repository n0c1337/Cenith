#ifndef CSTRINGS_H
#define CSTRINGS_H

#include "allocator.h"
#include <string.h>
#include <ctype.h>

char* string_new(Allocator* allocator, size_t size);
char* string_from(Allocator* allocator, char* str);
void string_delete(Allocator* allocator, char* str);
char* string_resize(Allocator* allocator, char* str, size_t new_size);
void string_clear(char *str);
int string_is_alphanumeric(char* str);

#endif 