#include "../include/parser.h"

int main() {
    allocator_initialize(&global_allocator, 40);

    char* content = parser_parse_file_content("test.ccc");
    //ir_initialize("test.ir");
    parser_initialize(content);
    parser_parse_function();
    //ir_cleanup();

    allocator_cleanup(&global_allocator);
    return 0;
}