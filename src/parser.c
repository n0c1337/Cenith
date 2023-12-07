#include "../include/parser.h"

Token* current_token = NULL;
char* content = NULL;
int content_character_idx = 0;

char* parser_parse_file_content(char* filename) {
    FILE* file = file_open(filename, "r");

    char* content = file_read(file);

    file_close(file);

    return content;
}

void parser_initialize(char* to_be_parsed) {
    content = to_be_parsed;
    if (current_token == NULL)
        current_token = allocator_alloc(&global_allocator, sizeof(Token));
}

void parser_parse_next() {
    char* token = strtok(content + content_character_idx, " ");
    printf("%s\n", token);
    if (token != NULL) {
        *current_token = lexer_tokenize(token);
        content_character_idx += strlen(token) + 1;
    } else {
        current_token->type = TOKEN_INVALID;
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
    match(TOKEN_BracketOpen);

    // Parse arguments and their types
    while (current_token->type != TOKEN_BracketClosed) {
        match(TOKEN_IdentifierType);
        //match(TOKEN_IdentifierVariable);
    }
    match(TOKEN_BracketClosed);
    match(TOKEN_CurlyBracketOpen);

    // Parse body
    while (current_token->type != TOKEN_CurlyBracketClosed) {
        //parse_statement();
        parser_parse_next();
        printf("Body\n");
    }

    //match(TOKEN_CurlyBracketClosed); // This will seg fault because we've already reached the last curly bracket :)
}

void parser_parse_if_statement() {
    match(TOKEN_KeywordIf);
    match(TOKEN_BracketOpen);

    while (current_token->type != TOKEN_BracketClosed) {
        match(TOKEN_IdentiferCondition);
    }

    match(TOKEN_BracketClosed);
    match(TOKEN_CurlyBracketOpen);

    // Parse body
    while (current_token->type != TOKEN_CurlyBracketClosed) {
        printf("Body\n");
    }

    //match(TOKEN_CurlyBracketClosed); // This will seg fault because we've already reached the last curly bracket :)
}
