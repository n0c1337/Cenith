#include "../include/ir.h"

char* x64_calling_convention[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
char* x32_calling_convention[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
char* x16_calling_convention[] = {"di", "si", "dx", "cx", "r8w", "r9w"};
char* x8_calling_convention[] = {"dil", "sil", "dl", "cl", "r8b", "r9b"};
char* floating_point_calling_convention[] = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"};

char* for_loop_construct =
    "JMP .L%d" // Jump to the second label (compare label)
    ".L%d:"
    "<body>"
    ".L%d:"
    "CMP %s, %s"
    "%s, .L%d"; // Jump to the first label (body label)

char* if_statement_construct = 
    "CMP %s, %s"
    "%s, .L%d" // Opposite of the compare condition (jg -> jl | jl -> jg)
    "<body>"
    ".L%d:";

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
        char* variable_identifier = ir_translate_variable(token);
        file_write_line(file, variable_identifier);
    }
}

char* ir_translate_function(Token* token) {
    char* function_identifier = string_new(&global_allocator, strlen(token->content) + 1);
    sprintf(function_identifier, "%s:", token->content);
    return function_identifier;
}

char* ir_translate_variable(Token* token) {
    char* variable = string_new(&global_allocator, strlen(token->content) + 10);
    sprintf(variable, "\tVAR %s, %ld", token->content, token->size);
    return variable;
}

char* ir_translate_if_statement(Token* token) {

}

char* ir_translate_for_loop(Token* token) {

}

char* ir_translate_while_loop(Token* token) {
    
}