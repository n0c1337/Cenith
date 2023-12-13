#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "cstrings.h"

typedef enum tokens_t {
    TOKEN_INVALID = -1,
    // Keywords
    TOKEN_KEYWORD_i8,
    TOKEN_KEYWORD_i16,
    TOKEN_KEYWORD_i32,
    TOKEN_KEYWORD_i64,
    TOKEN_KEYWORD_float,
    TOKEN_KEYWORD_if,
    TOKEN_KEYWORD_for,
    TOKEN_KEYWORD_while,
    TOKEN_KEYWORD_return,
    // identifier
    TOKEN_IDENTIFIER,
    TOKEN_IDENTIFIER_TYPE,
    TOKEN_IDENTIFIER_POINTER,
    // Literals
    TOKEN_LITERAL_INTEGER,
    TOKEN_LITERAL_STRING,
    TOKEN_LITERAL_FLOAT,
    // Grammatic characters
    TOKEN_GC_PARENTHESES_OPEN,
    TOKEN_GC_PARENTHESES_CLOSED,
    TOKEN_GC_CURLY_BRACKET_OPEN,
    TOKEN_GC_CURLY_BRACKET_CLOSED,
    TOKEN_GC_SQUARE_BRACKET_OPEN,
    TOKEN_GC_SQUARE_BRACKET_CLOSED,
    TOKEN_GC_SEMICOLON,
    // Function related
    TOKEN_FUNCTION_CALL,
    // Operators
    TOKEN_OPERATOR_Plus,
    TOKEN_OPERATOR_Minus,
    TOKEN_OPERATOR_Multiply,
    TOKEN_OPERATOR_Division,
    TOKEN_OPERATOR_Assignment,
} Tokens;

typedef struct token_t {
    // The exact type of the Token
    Tokens type;
    // Tokens value (only used for control structures)
    char* value;
    // Size of the Token
    long size;
    Tokens type_identifier;
} Token;

typedef struct mapping_t {
    const char* keyword;
    Tokens type;
    Tokens type_identifier;
} Mapping;

Token lexer_is_keyword(char* word);
Token lexer_tokenize(char* content);
Token lexer_tokenize_single(char ch);

#endif 