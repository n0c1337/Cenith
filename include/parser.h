#ifndef PARSER_H
#define PARSER_H

#include "cstrings.h"
#include "file.h"
#include "lexer.h"

char* parser_parse_file_content(char* filename);
void parser_initialize(char* to_be_parsed);
void parser_parse_next();
void match(Tokens expected_token);
void parser_parse_function();

#endif