#include "../include/file.h"

FILE* file_open(char* filename, char* permissions) {
    return fopen(filename, permissions);
}

char* file_read_line(FILE* file, int line) {
    return NULL;
}

char* file_read(FILE* file, Allocator* allocator) {
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* file_content = string_new(allocator, file_size);
    fread(file_content, 1, file_size, file);

    return file_content;
}

void file_write_line(FILE* file, char* content) {
    fseek(file, 0, SEEK_END);
    fprintf(file, "%s\n", content);
    fflush(file);
}

void file_write(FILE* file, char* content) {
    fprintf(file, "%s", content);
    fflush(file);
}

void file_close(FILE* file) {
    fclose(file);
}