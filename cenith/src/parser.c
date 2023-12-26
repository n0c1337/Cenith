#include "../include/parser.h"

Token* current_token = NULL;
char* content = NULL;
int content_character_idx = 0;

char* parser_parse_file_content(char* filename) {
    FILE* file = file_open(filename, "r");

    char* content = file_read(file);
    string_remove_new_lines(content);
    printf("%s\n", content);

    file_close(file);

    return content;
}

void parser_initialize(char* to_be_parsed) {
    content = to_be_parsed;
    if (current_token == NULL)
        current_token = allocator_alloc(&global_allocator, sizeof(Token));
}

void parser_parse_next() {
    int index = 0;
    char* buffer = string_new(&global_allocator, strlen(content));

    for (int i = content_character_idx; i < strlen(content); i++) {
        if(content[i] == ' ') {
            buffer[index] = '\0';
            printf("%s\n", buffer);
            *current_token = lexer_tokenize(buffer);
            printf("%s -> %i\n", current_token->content, current_token->type);
            if (current_token->type != TOKEN_INVALID && current_token->type_identifier != TOKEN_INVALID) {
                ir_translate_to_ir(current_token);
            }
            index = 0;
            content_character_idx = i + 1;
            break;
        } else {
            buffer[index++] = content[i];
        }
    }
}

// Match a specific token type or a tokens identifier
void match(Tokens expected_token) {
    if (expected_token == current_token->type || expected_token == current_token->type_identifier) {
        parser_parse_next();
    } else {
        printf("Error: Expected token %d, but got token %d\n", expected_token, current_token->type);
        allocator_destroy(&global_allocator);
        exit(-1);
    }
}

void parser_parse_function() {
    parser_parse_next();
    match(TOKEN_IDENTIFIER_TYPE);
    match(TOKEN_IDENTIFIER);
    match(TOKEN_GC_PARENTHESES_OPEN);

    // Parse arguments and their types
    while (current_token->type != TOKEN_GC_PARENTHESES_CLOSED) {
        match(TOKEN_IDENTIFIER_TYPE);
        match(TOKEN_IDENTIFIER);
    }
    match(TOKEN_GC_PARENTHESES_CLOSED);
    match(TOKEN_GC_CURLY_BRACKET_OPEN);

    // Parse body
    parser_parse_body();
}

void parser_parse_body() {
    while (current_token->type != TOKEN_GC_CURLY_BRACKET_CLOSED) {
        parser_parse_next();

        if (current_token->type == TOKEN_KEYWORD_if) { // Parse if statement
            parser_parse_if_statement();
        } else if (current_token->type == TOKEN_KEYWORD_for) { // Parse for loop
            parse_parse_for_statement();
        } else if (current_token->type == TOKEN_KEYWORD_while) { // Parse while loop

        }
    }

    match(TOKEN_GC_CURLY_BRACKET_CLOSED);
}

void parser_parse_if_statement() {
    parser_parse_next();
    match(TOKEN_GC_PARENTHESES_OPEN);

    while (current_token->type != TOKEN_GC_PARENTHESES_CLOSED) {
        parser_parse_next();
    }

    match(TOKEN_GC_PARENTHESES_CLOSED);
    match(TOKEN_GC_CURLY_BRACKET_OPEN);

    // Parse body
    parser_parse_body();
}

void parse_parse_for_statement() {
    parser_parse_next();
    match(TOKEN_GC_PARENTHESES_OPEN);

    while (current_token->type != TOKEN_GC_PARENTHESES_CLOSED) {
        parser_parse_next();
    }

    match(TOKEN_GC_PARENTHESES_CLOSED);
    match(TOKEN_GC_CURLY_BRACKET_OPEN);

    // Parse body
    parser_parse_body();
}