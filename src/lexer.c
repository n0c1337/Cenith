#include "../include/lexer.h"

Token lexer_is_keyword(char* word) {
    Token token = { -1, NULL, -1, -1 };

    Mapping keyword_mapping[] = {
        {"i8", TOKEN_KEYWORD_i8, TOKEN_IDENTIFIER_TYPE},
        {"i16", TOKEN_KEYWORD_i16, TOKEN_IDENTIFIER_TYPE},
        {"i32", TOKEN_KEYWORD_i32, TOKEN_IDENTIFIER_TYPE},
        {"i64", TOKEN_KEYWORD_i64, TOKEN_IDENTIFIER_TYPE},
        {"float", TOKEN_KEYWORD_float, TOKEN_IDENTIFIER_TYPE},
        {"if", TOKEN_KEYWORD_if, TOKEN_IDENTIFIER_TYPE},
        {"for", TOKEN_KEYWORD_for, TOKEN_IDENTIFIER_TYPE},
        {"while", TOKEN_KEYWORD_while, TOKEN_IDENTIFIER_TYPE},
        {"return", TOKEN_KEYWORD_return, TOKEN_IDENTIFIER_TYPE},
        {NULL, TOKEN_INVALID, TOKEN_INVALID}
    };

    for (int i = 0; keyword_mapping[i].keyword != NULL; i++) {
        if (strcmp(word, keyword_mapping[i].keyword) == 0) {
            token.type = keyword_mapping[i].type;
            token.type_identifier = keyword_mapping[i].type_identifier;
            return token;
        }
    }

    return token;
}

Token lexer_tokenize(char* content) {
    Token token = { -1, NULL, -1, -1 };
    
    // Check if token is a keyword
    token = lexer_is_keyword(content);
    
    // Check if token is a identifier
    if (token.type == TOKEN_INVALID && string_is_alphanumeric(content)) {
            token.type = TOKEN_IDENTIFIER;
            token.value = content;
    }

    // Check if token is an integer literal
    if (token.type == TOKEN_INVALID) {
        int i = 0;
        while (content[i])
        {
            if (!isdigit(content[i])) {
                break;
            }
            i++;
        }

        if (content[i] == '\0') {
            token.type = TOKEN_LITERAL_INTEGER;
        }
    }

    // Check if token is a single grammatical character
    if (token.type == TOKEN_INVALID)
        token = lexer_tokenize_single(content[0]);

    return token;
}

Token lexer_tokenize_single(char ch) {
    Token token = { -1, NULL, -1 };
    switch (ch) {
        case ';':
            token.type = TOKEN_GC_SEMICOLON;
            break;
        case '(':
            token.type = TOKEN_GC_PARENTHESES_OPEN;
            break;
        case ')':
            token.type = TOKEN_GC_PARENTHESES_CLOSED;
            break;
        case '{':
            token.type = TOKEN_GC_CURLY_BRACKET_OPEN;
            break;
        case '}':
            token.type = TOKEN_GC_CURLY_BRACKET_CLOSED;
            break;
        case '[':
            token.type = TOKEN_GC_SQUARE_BRACKET_OPEN;
            break;
        case ']':
            token.type = TOKEN_GC_SQUARE_BRACKET_CLOSED;
            break;
        case '+':
            token.type = TOKEN_OPERATOR_Plus;
            break;
        case '-':
            token.type = TOKEN_OPERATOR_Minus;
            break;
        case '*':
            token.type = TOKEN_OPERATOR_Multiply;
            break;
        case '/':
            token.type = TOKEN_OPERATOR_Division;
            break;
        case '=':
            token.type = TOKEN_OPERATOR_Assignment;
            break;
    }
    return token;
}