#ifndef IR_H
#define IR_H

#include "file.h"
#include "lexer.h"
#include "cstrings.h"

void ir_initialize(char* filename);
void ir_cleanup();
void ir_translate_to_ir(Token* token);
char* ir_translate_function(Token* token);
char* ir_translate_function_parameter(Token* token);
char* ir_translate_variable(Token* token);
char* ir_translate_if_statement(Token* token);
char* ir_translate_for_loop(Token* token);
char* ir_translate_while_loop(Token* token);

#endif