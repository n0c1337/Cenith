#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "../include/cstrings.h"

FILE* file_open(char* filename, char* permissions);
char* file_read_line(FILE* file, int line);
char* file_read(FILE* file);
void file_write_line(FILE* file, char* content);
void file_write(FILE* file, char* content);
void file_close(FILE* file);

#endif