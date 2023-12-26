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
    TOKEN_IDENTIFIER_COMPARISON,
    // Literals
    TOKEN_LITERAL_INTEGER,
    TOKEN_LITERAL_STRING,
    TOKEN_LITERAL_FLOAT,
    // Grammatical characters
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
    TOKEN_OPERATOR_PLUS,
    TOKEN_OPERATOR_MINUS,
    TOKEN_OPERATOR_MULTIPLY,
    TOKEN_OPERATOR_DIVISION,
    TOKEN_OPERATOR_ASSIGNMENT,
    // Comparison
    TOKEN_KEYWORD_LESS,
    TOKEN_KEYWRD_LESS_EQUALS,
    TOKEN_KEYWORD_EQUALS,
    TOKEN_KEYWORD_NOT_EQUALS,
    TOKEN_KEYWORD_GREATER,
    TOKEN_KEYWORD_GREATER_EQUALS,
} Tokens;

typedef struct token_t {
    Tokens type;
    char* content;
    long size;
    // Of which type a specific token is (e.g. (i8, i16) -> are TOKEN_IDENTIFIER_TYPE)
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