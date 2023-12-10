#include "../include/lexer.h"

Token lexer_tokenize(char* content) {
    Token token = { -1, -1, NULL };
    if (strcmp(content, "i8") == 0) {
        token.type = TOKEN_TypeInt8;
        token.identifier = TOKEN_IdentifierType;
    } else if (strcmp(content, "i16") == 0) {
        token.type = TOKEN_TypeInt16;
        token.identifier = TOKEN_IdentifierType;
    } else if (strcmp(content, "i32") == 0) {
        token.type = TOKEN_TypeInt32;
        token.identifier = TOKEN_IdentifierType;
    } else if (strcmp(content, "i64") == 0) {
        token.type = TOKEN_TypeInt64;
        token.identifier = TOKEN_IdentifierType;
    } else if (strcmp(content, "float") == 0) {
        token.type = TOKEN_TypeFloat;
        token.identifier = TOKEN_IdentifierType;
    } else if (strcmp(content, "double") == 0) {
        token.type = TOKEN_TypeDouble;
        token.identifier = TOKEN_IdentifierType;
    } else if (strcmp(content, "void") == 0) {
        token.type = TOKEN_TypeVoid;
        token.identifier = TOKEN_IdentifierType;
    } else if (strcmp(content, "return") == 0) {
        token.type = TOKEN_KeywordReturn;
        token.identifier = Token_IdentifierKeyword;
    } else if (strcmp(content, "if") == 0) {
        token.type = TOKEN_KeywordIf;
    } else if (string_is_alphanumeric(content) == 1) { // Needs to be one of the last compares so it won't get overwritten by others.
        token.type = TOKEN_IdentifierFuncton;
        token.identifier = TOKEN_IdentifierFuncton;
        token.value = content;
    } else if (string_is_alphanumeric_except_colon(content) == 1) {
        token.type = TOKEN_IdentifierVariable;
        token.identifier = TOKEN_IdentifierVariable;
        token.value = content;
    }

    if (strcmp(content, "==") == 0)
        token.identifier = TOKEN_IdentiferCondition;

    if (token.type == TOKEN_INVALID)
        token = lexer_tokenize_single(content[0]);

    return token;
}

Token lexer_tokenize_single(char ch) {
    Token token = { -1, -1, NULL };
    switch (ch) {
        case ';':
            token.type = TOKEN_TypeSemicolon;
            break;
        case '(':
            token.type = TOKEN_TypeBracketOpen;
            break;
        case ')':
            token.type = TOKEN_TypeBracketClosed;
            break;
        case '{':
            token.type = TOKEN_TypeCurlyBracketOpen;
            break;
        case '}':
            token.type = TOKEN_TypeCurlyBracketClosed;
            break;
        case '[':
            token.type = TOKEN_TypeSquareBracketOpen;
            break;
        case ']':
            token.type = TOKEN_TypeSquareBrackedClosed;
            break;
        case '+':
            token.type = TOKEN_OperatorPlus;
            break;
        case '-':
            token.type = TOKEN_OperatorMinus;
            break;
        case '*':
            token.type = TOKEN_OperatorMultiply;
            break;
        case '/':
            token.type = TOKEN_OperatorDivision;
            break;
        case '=':
            token.type = TOKEN_OperatorAssignment;
            break;
    }
    return token;
}