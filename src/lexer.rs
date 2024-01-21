use crate::tokens::{Tokens, Token};

pub struct Lexer;

impl Lexer {
    pub fn new() -> Self {
        Lexer
    }

    pub fn lex(&self, content: &str) -> Token {
        let mut token = Token::default();
        token.content = Some(content.to_string());

        // Check for types i8, i16, i32, i64, float and void
        if Tokens::from_type(content) != Tokens::Invalid {
            token.token_type = Tokens::from_type(content);
            token.ident = Tokens::IdentifierType;
            return token
        }

        // Check for keywords
        if Tokens::from_keyword(content) != Tokens::Invalid {
            token.token_type = Tokens::from_keyword(content);
            return token
        }

        // Check for arithmetic operations & bitwise operation
        if Tokens::from_operation_or_delimiter(content) != Tokens::Invalid {
            token.token_type = Tokens::from_operation_or_delimiter(content);
            return token
        }

        // Check for comparison operator
        if Tokens::from_comparison_operator(content) != Tokens::Invalid {
            token.token_type = Tokens::from_comparison_operator(content);
            return token
        }

        // Check for integer literal
        if let Ok(_) = content.parse::<i64>() {
            token.token_type = Tokens::LiteralInteger;
            token.ident = Tokens::LiteralInteger;
            return token
        }

        // Check for float literal
        if let Ok(_) = content.parse::<f64>() {
            token.token_type = Tokens::LiteralFloat;
            token.ident = Tokens::LiteralFloat;
            return token
        }

        if content.starts_with("&") {
            token.token_type = Tokens::OperatorReference;
            token.ident = Tokens::OperatorReference;
            return token
        }
        
        // Check for dereference
        if content.starts_with("@") {
            // Handle dereference
            return token
        }

        // Check for pointer
        if content.ends_with("*") {
            token.is_pointer = true;
            return token
        }

        // Check for function name or variable name
        if content.chars().all(char::is_alphanumeric) {
            token.token_type = Tokens::Identifier;
            token.ident = Tokens::Identifier;
            return token
        }

        token
    }
}
