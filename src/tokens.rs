#[derive(Clone, Debug, PartialEq)]
pub enum Tokens {
    Invalid = 0x0,
    // Keywords
    I8,
    I16,
    I32,
    I64,
    Float,
    Void,
    If,
    Else,
    For,
    While,
    Return,
    Extern,
    // Identifiers
    Identifier,
    IdentifierType,
    IdentifierArray,
    IdentifierPointer,
    IdentifierFunctionCall,
    // Literals
    LiteralInteger,
    LiteralFloat,
    LiteralString,
    LiteralArray,
    //Tokens::Comparison
    ComparisonLess,
    ComparisonLessEquals,
    ComparisonEquals,
    ComparisonNotEquals,
    ComparisonGreater,
    ComparisonGreaterEquals,
    // Operators
    OperatorPlus,
    OperatorMinus,
    OperatorMultiply,
    OperatorDivision,
    OperatorModulo,
    OperatorAssign,
    OperatorReference,
    OperatorDereference, // @
    // Bitwise
    OperatorBitwiseAnd,
    OperatorBitwiseOr,
    OperatorBitwiseXor,
    OperatorBitwiseNot,
    OperatorBitwiseSHL,
    OperatorBitwiseSHR,
    // Gramma
    RoundBracketOpen,
    RoundBracketClosed,
    SquareBracketOpen,
    SquareBracketClosed,
    CurlyBracketOpen,
    CurlyBracketClosed,
    Semicolon,
    Comma
}

impl Tokens {
    pub fn from_type(conent: &str) -> Self {
        match conent {
            "i8" => Tokens::I8,
            "i16" => Tokens::I16,
            "i32" => Tokens::I32,
            "i64" => Tokens::I64,
            "float" => Tokens::Float,
            "void" => Tokens::Void,
            _ => Tokens::Invalid,
        }
    }

    pub fn from_keyword(keyword: &str) -> Self {
        match keyword {
            "if" => Tokens::If,
            "else" => Tokens::Else,
            "for" => Tokens::For,
            "while" => Tokens::While,
            "return" => Tokens::Return,
            "extern" => Tokens::Extern,
            _ => Tokens::Invalid,
        }
    }

    pub fn from_operation_or_delimiter(content: &str) -> Self {
        match content {
            // Operations
            "+" => Tokens::OperatorPlus,
            "-" => Tokens::OperatorMinus,
            "*" => Tokens::OperatorMultiply,
            "/" => Tokens::OperatorDivision,
            "%" => Tokens::OperatorModulo,
            "=" => Tokens::OperatorAssign,
            "&" => Tokens::OperatorBitwiseAnd,
            "|" => Tokens::OperatorBitwiseOr,
            "^" => Tokens::OperatorBitwiseXor,
            "!" => Tokens::OperatorBitwiseNot,
            "<<" => Tokens::OperatorBitwiseSHL,
            ">>" => Tokens::OperatorBitwiseSHR,
            // Delimiter
            "(" => Tokens::RoundBracketOpen,
            ")" => Tokens::RoundBracketClosed,
            "{" => Tokens::CurlyBracketOpen,
            "}" => Tokens::CurlyBracketClosed,
            ";" => Tokens::Semicolon,
            "," => Tokens::Comma,
            _ => Tokens::Invalid,
        }
    }

    pub fn from_comparison_operator(content: &str) -> Self {
        match content {
            "<" => Tokens::ComparisonLess,
            "<=" => Tokens::ComparisonLessEquals,
            "==" => Tokens::ComparisonEquals,
            "!=" => Tokens::ComparisonNotEquals,
            ">" => Tokens::ComparisonGreater,
            ">=" => Tokens::ComparisonGreaterEquals,
            _ => Tokens::Invalid,
        }
    }
}

#[derive(Clone, Debug)]
pub struct Token {
    pub token_type: Tokens,
    pub ident: Tokens,
    pub content: Option<String>,
    pub is_pointer: bool,
}

impl Token {
    pub fn default() -> Token {
        Token { token_type: Tokens::Invalid, ident: Tokens::Invalid, content: None, is_pointer: false }
    }
}

pub fn is_delimiter_character(ch: char) -> bool {
    match ch {
        '(' => true,
        ')' => true,
        '{' => true,
        '}' => true,
        ';' => true,
        ',' => true,
        _ => false
    }
}

pub fn get_token_size(token: Tokens) -> i32 {
    match token {
        Tokens::I8 => 1,
        Tokens::I16 => 2,
        Tokens::I32 => 4,
        Tokens::I64 => 8,
        _ => 0
    }
}

pub fn get_token_stack_size(size: i32) -> &'static str {
    match size {
        1 => "BYTE",
        2 => "WORD",
        4 => "DWORD",
        8 => "QWORD",
        _ => "Invalid"
    }
}

pub fn get_right_sized_register(size: i32) -> &'static str {
    match size {
        1 => "r10b",
        2 => "r10w",
        4 => "r10d",
        8 => "r10", 
        _ => "r10" // Crazy performance improvment always returning a 64 Bit Register.
    }
}

// Multi-purpose register definitions
pub const LEA_REGISTER: &str = "r11";