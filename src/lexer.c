#include "../include/lexer.h"

Allocator global_allocator = { NULL };

Token lexer_lex(char* content) {
    char* token_content = string_from(&global_allocator, content);
    Token token = { TOKEN_INVALID, TOKEN_INVALID, NULL, -1, 0 };

    if (strlen(content) == 0) {
        return token;
    }

    token = lexer_is_keyword(content);

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
            token.identifier = TOKEN_LITERAL_INTEGER;
        }
    }

    if (token.type == TOKEN_INVALID && string_is_alphanumeric(content)) {
        token.identifier = TOKEN_IDENTIFIER;
        token.type = TOKEN_IDENTIFIER;
    }

    if (token.type == TOKEN_INVALID) {
        int length = strlen(content);
        if (length > 1 && content[length - 1] == '*') {
            token.type = TOKEN_IDENTIFIER_TYPE;
            token.identifier = TOKEN_IDENTIFIER_POINTER;
            token.is_pointer = 1;
            token.content[length - 1] = '\0';
        }
    }

    if (token.type == TOKEN_INVALID) {
        token = lexer_lex_single(content[0]);
    }

    token.content = token_content;

    return token;
}

Token lexer_is_keyword(char* keyword) {
    Token token = { -1, -1, NULL, -1, 0 };

    Mapping keyword_mapping[] = {
        {"i8", TOKEN_KEYWORD_I8, TOKEN_IDENTIFIER_TYPE},
        {"i16", TOKEN_KEYWORD_I16, TOKEN_IDENTIFIER_TYPE},
        {"i32", TOKEN_KEYWORD_I32, TOKEN_IDENTIFIER_TYPE},
        {"i64", TOKEN_KEYWORD_I64, TOKEN_IDENTIFIER_TYPE},
        {"void", TOKEN_KEYWORD_VOID, TOKEN_IDENTIFIER_TYPE},
        {"float", TOKEN_KEYWORD_FLOAT, TOKEN_IDENTIFIER_TYPE},
        {"if", TOKEN_KEYWORD_IF, TOKEN_IDENTIFIER_CONTROL_STRUCTURE},
        {"else", TOKEN_KEYWORD_ELSE, TOKEN_IDENTIFIER_CONTROL_STRUCTURE},
        {"for", TOKEN_KEYWORD_FOR, TOKEN_IDENTIFIER_CONTROL_STRUCTURE},
        {"while", TOKEN_KEYWORD_WHILE, TOKEN_IDENTIFIER_CONTROL_STRUCTURE},
        {"return", TOKEN_KEYWORD_RETURN, TOKEN_KEYWORD_RETURN},
        {"extern", TOKEN_KEYWORD_EXTERN, TOKEN_KEYWORD_EXTERN},
        {"<", TOKEN_KEYWORD_LESS, TOKEN_IDENTIFIER_COMPARISON},
        {"<=", TOKEN_KEYWORD_LESS_EQUALS, TOKEN_IDENTIFIER_COMPARISON},
        {"==", TOKEN_KEYWORD_EQUALS, TOKEN_IDENTIFIER_COMPARISON},
        {"!=", TOKEN_KEYWORD_NOT_EQUALS, TOKEN_IDENTIFIER_COMPARISON},
        {">", TOKEN_KEYWORD_GREATER, TOKEN_IDENTIFIER_COMPARISON},
        {">=", TOKEN_KEYWORD_GREATER_EQUALS, TOKEN_IDENTIFIER_COMPARISON},
        {"<<", TOKEN_KEYWORD_BITWISE_SHL, TOKEN_INVALID},
        {">>", TOKEN_KEYWORD_BITWISE_SHR, TOKEN_INVALID},
        {"++", TOKEN_KEYWORD_PLUSPLUS, TOKEN_INVALID},
        {"--", TOKEN_KEYWORD_MINUSMINUS, TOKEN_INVALID},
        {NULL, TOKEN_INVALID, TOKEN_INVALID}
    };

    for (int i = 0; keyword_mapping[i].content != NULL; i++) {
        if (string_compare(keyword, keyword_mapping[i].content) == 1) {
            token.type = keyword_mapping[i].type;
            token.identifier = keyword_mapping[i].identifier;
            token.is_pointer = 0;
            return token;
        }
    }

    return token;
}

Token lexer_lex_single(char ch) {
    Token token = { -1, -1, NULL, -1, 0 };
    switch (ch) {
        case ';':
            token.type = TOKEN_KEYWORD_SEMICOLON;
            break;
        case '(':
            token.type = TOKEN_KEYWORD_ROUND_BRACKET_OEPN;
            break;
        case ')':
            token.type = TOKEN_KEYWORD_ROUND_BRACKET_CLOSED;
            break;
        case '{':
            token.type = TOKEN_KEYWORD_CURLY_BRACKET_OPEN;
            break;
        case '}':
            token.type = TOKEN_KEYWORD_CURLY_BRACKET_CLOSED;
            break;
        case '[':
            token.type = TOKEN_KEYWORD_SQUARED_BRACKET_OPEN;
            break;
        case ']':
            token.type = TOKEN_KEYWORD_SQUARED_BRACKET_CLOSED;
            break;
        case '+':
            token.type = TOKEN_KEYWORD_PLUS;
            break;
        case '-':
            token.type = TOKEN_KEYWORD_MINUS;
            break;
        case '*':
            token.type = TOKEN_KEYWORD_MULTIPLY;
            break;
        case '/':
            token.type = TOKEN_KEYWORD_DIVISION;
            break;
        case '=':
            token.type = TOKEN_KEYWORD_ASSIGNMENT;
            break;
        case '&':
            token.type = TOKEN_KEYWORD_BITWISE_AND;
            break;
        case '|':
            token.type = TOKEN_KEYWORD_BITWISE_OR;
            break;
        case '^':
            token.type = TOKEN_KEYWORD_BITWISE_XOR;
            break;
        case '!':
            token.type = TOKEN_KEYWORD_BITWISE_NOT;
            break;
    }
    return token;
}

int lexer_is_delimiter_char(char ch) {
    return (isspace(ch) || ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ';');
}