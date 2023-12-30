#include <stdio.h>
#include "../include/parser.h"

int main() {
    allocator_initialize(&global_allocator);

    FILE* file = file_open("test.cenith", "r");

    char* content = file_read(file, &global_allocator);
    printf("%s\n", content);

    file_close(file);

    parser_initialize(content);

    parse_function();

    allocator_destroy(&global_allocator);
    return 0;
}