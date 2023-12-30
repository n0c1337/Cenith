#include "../include/parser.h"

Token* previous_token = NULL;
Token* current_token = NULL;
char* g_content = NULL;
char* g_buffer = NULL;
int g_content_character_idx = 0;

typedef struct ContextStack_t {
    int depth;
} ContextStack;

ContextStack context_stack;

void push_context() {
    context_stack.depth++;
}

void pop_context() {
    context_stack.depth--;
}

void parser_initialize(char* to_be_paresd) {
    g_content = to_be_paresd;
    string_remove_new_lines(g_content);
    g_buffer = string_new(&global_allocator, strlen(g_content));
    if (current_token == NULL)
        current_token = allocator_alloc(&global_allocator, sizeof(Token));
    if (previous_token == NULL)
        previous_token = allocator_alloc(&global_allocator, sizeof(Token));
}

void parser_parse_next() {
    int index = 0;

    while (isspace(g_content[g_content_character_idx])) {
        g_content_character_idx++;
    }

    if (g_content[g_content_character_idx] == '\0') {
        return;
    }

    if (lexer_is_delimiter_char(g_content[g_content_character_idx])) {
        g_buffer[index++] = g_content[g_content_character_idx++];
    } else {
        while (!isspace(g_content[g_content_character_idx]) && g_content[g_content_character_idx] != '\0' && !lexer_is_delimiter_char(g_content[g_content_character_idx])) {
            g_buffer[index++] = g_content[g_content_character_idx++];
        }
    }

    if (index > 0) {
        g_buffer[index] = '\0';
        *previous_token = *current_token;
        *current_token = lexer_lex(g_buffer);
        //printf("%s -> %i\n", current_token->content, current_token->type);
        string_clear(g_buffer);
    }
}

#define next() parser_parse_next()

void match(Tokens expected_token) {
    if (expected_token == current_token->type || expected_token == current_token->identifier) {
        next();
    } else {
        printf("Error: Expected token %d, but got token %s(%d)\n", expected_token, current_token->content, current_token->type);
        allocator_destroy(&global_allocator);
        exit(-1);
    }
}

void parse_function() {
    next();
    match(TOKEN_IDENTIFIER_TYPE);
    match(TOKEN_IDENTIFIER);
    match(TOKEN_KEYWORD_ROUND_BRACKET_OEPN);

    push_context();
    
    while (current_token->type != TOKEN_KEYWORD_ROUND_BRACKET_CLOSED) {   
        match(TOKEN_IDENTIFIER_TYPE);
        match(TOKEN_IDENTIFIER);
    }

    match(TOKEN_KEYWORD_ROUND_BRACKET_CLOSED);

    parse_body();

    pop_context();
}

void parse_body() {
    match(TOKEN_KEYWORD_CURLY_BRACKET_OPEN);

    push_context();

    while (current_token->type != TOKEN_KEYWORD_CURLY_BRACKET_CLOSED) {        
        if (current_token->type == TOKEN_KEYWORD_CURLY_BRACKET_OPEN) {
            parse_body();
        } else if (current_token->type == TOKEN_KEYWORD_FOR) {
            parse_for_loop();
        } else if (current_token->type == TOKEN_KEYWORD_IF) {
            parse_if_statement();
        } else if (current_token->identifier == TOKEN_IDENTIFIER_TYPE) {
            parse_variable();
        } else if (current_token->type == TOKEN_KEYWORD_RETURN) {
            parse_return();
        }
        next();
    }

    match(TOKEN_KEYWORD_CURLY_BRACKET_CLOSED);

    pop_context();
}

void parse_for_loop() {
    next();
    match(TOKEN_KEYWORD_ROUND_BRACKET_OEPN);

    parse_variable();
    printf(".L%d:\n", context_stack.depth);

    next();
    match(TOKEN_KEYWORD_SEMICOLON);

    char** condition = parse_condition();

    next();
    match(TOKEN_KEYWORD_SEMICOLON);

    parse_statement();
    
    next();
    match(TOKEN_KEYWORD_ROUND_BRACKET_CLOSED);
    parse_body();
    
    printf("ADD i, 1\n");
    printf("CMP %s, %s\n", condition[0], condition[2]);
    printf("%s .L%d\n", condition[1], context_stack.depth);
}

void parse_if_statement() {
    next();
    match(TOKEN_KEYWORD_ROUND_BRACKET_OEPN);
}

void parse_variable() {
    next();
    
    char* variable_name = NULL;
    if (current_token->identifier == TOKEN_IDENTIFIER) {
        variable_name = string_from(&global_allocator, current_token->content);
    }

    next();
    match(TOKEN_KEYWORD_ASSIGNMENT);

    printf("ASSIGN %s, %s\n", variable_name, current_token->content);
}

char** parse_condition() {
    char* comperand_1 = NULL;
    char* condition = NULL;
    char* comperand_2 = NULL;

    if (current_token->identifier == TOKEN_IDENTIFIER || current_token->identifier == TOKEN_LITERAL_INTEGER) {
        comperand_1 = string_from(&global_allocator, current_token->content);
    }

    next();
    condition = parse_comparision();
    next();
   
    if (current_token->identifier == TOKEN_IDENTIFIER || current_token->identifier == TOKEN_LITERAL_INTEGER) {
        comperand_2 = string_from(&global_allocator, current_token->content);        
    }
    
    char** return_value = allocator_alloc(&global_allocator, 3 * sizeof(char*));
    return_value[0] = comperand_1;
    return_value[1] = condition;
    return_value[2] = comperand_2;

    return return_value;
}

char* parse_comparision() {
    char* condition = NULL;
    switch (current_token->type)
    {
    case TOKEN_KEYWORD_LESS:
        condition = string_from(&global_allocator, "JL");
        break;
    case TOKEN_KEYWORD_LESS_EQUALS:
        condition = string_from(&global_allocator, "JLE");
        break;
    case TOKEN_KEYWORD_EQUALS:
        condition = string_from(&global_allocator, "JE");
        break;
    case TOKEN_KEYWORD_NOT_EQUALS:
        condition = string_from(&global_allocator, "JNE");
        break;
    case TOKEN_KEYWORD_GREATER:
        condition = string_from(&global_allocator, "JG");
        break;
     case TOKEN_KEYWORD_GREATER_EQUALS:
        condition = string_from(&global_allocator, "JGE");
        break;
    
    default:
        break;
    }

    return condition;
}

void parse_statement() {
    if (current_token->identifier == TOKEN_IDENTIFIER) {
        char* variable = string_from(&global_allocator, current_token->content);
        next();

        // TODO: Handle more types
        switch (current_token->type)
        {
        case TOKEN_KEYWORD_PLUSPLUS:
            printf("ADD %s, 1\n", variable);
            break;
        case TOKEN_KEYWORD_MINUSMINUS:
            printf("SUB %s, 1\n", variable);
        default:
            break;
        }
    }
}

void parse_return() {
    next();

    switch (current_token->identifier) 
    {
    case TOKEN_LITERAL_INTEGER:
        printf("RET %s\n", current_token->content);
        break;
    case TOKEN_LITERAL_STRING:
        printf("RET %s\n", current_token->content);
        break;
    case TOKEN_IDENTIFIER:
        printf("RET %s\n", current_token->content);
        break;
    default:
        printf("RET\n");
        break;
    }
}