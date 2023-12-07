#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "cstrings.h"

typedef enum _Tokens {
    TOKEN_INVALID = -1,
    TOKEN_VALID,
    TOKEN_SPACE,
    TOKEN_TypeInt8,
    TOKEN_TypeInt16,
    TOKEN_TypeInt32,
    TOKEN_TypeInt64,
    TOKEN_TypeFloat,
    TOKEN_TypeDouble,
    TOKEN_TypeVoid,
    TOKEN_Semicolon,
    TOKEN_ValueNumber,
    TOKEN_ValueFloat,
    TOKEN_IdentifierType,
    TOKEN_IdentifierVariable,
    TOKEN_IdentifierFuncton,
    TOKEN_IdentiferCondition,
    Token_IdentifierKeyword,
    TOKEN_BracketOpen,
    TOKEN_BracketClosed,
    TOKEN_CurlyBracketOpen,
    TOKEN_CurlyBracketClosed,
    TOKEN_SquareBracketOpen,
    TOKEN_SquareBrackedClosed,
    TOKEN_KeywordIf,
    TOKEN_KeywordElse,
    TOKEN_KeywordFor,
    TOKEN_KeywordWhile,
    TOKEN_KeywordReturn,
    TOKEN_KeywordAssembly,
    TOKEN_FunctionCall,
    TOKEN_OperatorPlus,
    TOKEN_OperatorMinus,
    TOKEN_OperatorMultiply,
    TOKEN_OperatorDivision
} Tokens;

typedef struct _Token {
    // The parent type of a the Token (e.g. i8, i16 are "Type Identifiers")
    Tokens identifier;
    // The exact type of the Token
    Tokens type;
    // Tokens value (only used for control structures)
    char* value;
} Token;

Token lexer_tokenize(char* content);
Token lexer_tokenize_single(char ch);

#endif 