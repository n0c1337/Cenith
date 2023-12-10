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
            printf("Buffer: %s\n", buffer);
            *current_token = lexer_tokenize(buffer);
            index = 0;
            content_character_idx = i+1;
            break;
        } else {
            buffer[index++] = content[i];
        }
    }
}

void match(Tokens expected_token) {
    if (expected_token == current_token->type || expected_token == current_token->identifier) {
        parser_parse_next();
    } else {
        printf("Error: Expected token %d, but got token %d\n", expected_token, current_token->type);
        allocator_cleanup(&global_allocator);
        exit(-1);
    }
}

void parser_parse_function() {
    parser_parse_next();
    match(TOKEN_IdentifierType);
    match(TOKEN_IdentifierFuncton);
    match(TOKEN_TypeBracketOpen);

    // Parse arguments and their types
    while (current_token->type != TOKEN_TypeBracketClosed) {
        match(TOKEN_IdentifierVariable);
        match(TOKEN_IdentifierType);
    }
    match(TOKEN_TypeBracketClosed);
    match(TOKEN_TypeCurlyBracketOpen);

    // Parse body
    parser_parse_body();
}

void parser_parse_body() {
    while (current_token->type != TOKEN_TypeCurlyBracketClosed) {
        parser_parse_next();

        if (current_token->type == TOKEN_KeywordIf) { // Parse if statement
            parser_parse_if_statement();
        } else if (current_token->type == TOKEN_KeywordFor) { // Parse for loop

        } else if (current_token->type == TOKEN_KeywordWhile) { // Parse while loop

        }
    }
}

void parser_parse_if_statement() {
    parser_parse_next();
    match(TOKEN_TypeBracketOpen);

    while (current_token->type != TOKEN_TypeBracketClosed) {
        parser_parse_next();
    }

    match(TOKEN_TypeBracketClosed);
    match(TOKEN_TypeCurlyBracketOpen);

    // Parse body
    parser_parse_body();
}
