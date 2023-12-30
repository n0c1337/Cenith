#ifndef PARSER_H
#define PARSER_H

#include <file.h>

#include "lexer.h"

void parser_initialize(char* to_be_paresd);
void parser_parse_next();

void parse_function();
void parse_body();
void parse_for_loop();
void parse_if_statement();
void parse_variable();
char** parse_condition();
char* parse_comparision();
void parse_statement();
void parse_return();

#endif