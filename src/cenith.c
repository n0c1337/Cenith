#include "../include/parser.h"

int main() {
    allocator_initialize(&global_allocator, 10);

    char* content = parser_parse_file_content("test.c");
    parser_initialize(content);
    parser_parse_function();

    allocator_cleanup(&global_allocator);
    return 0;
}