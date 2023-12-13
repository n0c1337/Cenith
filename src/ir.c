#include "../include/ir.h"

FILE* file = NULL;

void ir_initialize(char* filename) {
    file = file_open(filename, "w");
}

void ir_cleanup() {
    file_close(file);
}

void ir_translate_to_ir(Token* token) {

    if (token->type_identifier == TOKEN_IDENTIFIER) {
        char* function_identifier = ir_translate_function(token);
        file_write_line(file, function_identifier);
    }
    
    else if (token->type_identifier == TOKEN_IDENTIFIER_TYPE) {
        char* variable_identifier = ir_translate_function_parameter(token);
        file_write_line(file, variable_identifier);
    }
}

char* ir_translate_function(Token* token) {
    char* function_identifier = string_new(&global_allocator, strlen(token->value) + 1);
    sprintf(function_identifier, "%s:", token->value);
    return function_identifier;
}

char* ir_translate_function_parameter(Token* token) {
    char* function_parameter = string_new(&global_allocator, strlen(token->value) + 1);
    sprintf(function_parameter, "\tLOAD %s", token->value);
    return function_parameter;
}

char* ir_translate_variable(Token* token, int global) {
    char* variable = string_new(&global_allocator, strlen(token->value) + 1);
    if (global == 1) {
        sprintf(variable, "GVAR %s, %s", token->value, token->size);
    } else {
        sprintf(variable, "\tVAR %s, %s", token->value, token->size);
    }

    return variable;
}

char* ir_translate_if_statement(Token* token) {

}

char* ir_translate_for_loop(Token* token) {

}

char* ir_translate_while_loop(Token* token) {
    
}