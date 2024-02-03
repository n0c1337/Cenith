use std::{fs, process::exit};

use crate::{lexer::Lexer, tokens::{Tokens, is_delimiter_character, Token}, instruction::Instruction};

#[derive(Clone, Debug)]
pub struct Parser {
    current_token: Box<Token>,
    pub instructions: Vec<Instruction>,
    start_index: usize,
    depth: usize
}

impl Parser {
    pub fn new() -> Self {
        Parser { current_token: Box::new(Token::default()), instructions: Vec::new(), start_index: 0, depth: 0 }
    }

    pub fn parse(&mut self) {
        let file_content = fs::read_to_string("./tests/test.cenith").expect("Couldn't read file.");

        if self.start_index > file_content.len() {
            return;
        }

        let mut token = String::new();

        if file_content.chars().nth(self.start_index).unwrap().is_whitespace() {
            self.start_index += 1;
        }

        for c in file_content.chars().skip(self.start_index) {
            if c.is_whitespace() {
                break;
            }

            if is_delimiter_character(c) {
                if token.is_empty() {
                    token.push(c);
                    self.start_index += 1;
                }
                break;
            }

            token.push(c);
            self.start_index += 1;
        }

        *self.current_token = *Box::new(Lexer::lex(&token));
        println!("{:?}", self.current_token)
    }

    pub fn expect_check(&mut self, token: Tokens) -> bool {
        if self.current_token.token_type == token || self.current_token.ident == token {
            self.parse();
            true
        } else {
            false
        }
    }

    pub fn expect(&mut self, token: Tokens) {
        if !self.expect_check(token.clone()) {
            println!("Expected token \x1b[91m{:?}\x1b[0m, but got \x1b[92m{:?}\x1b[0m \nPlease make sure you are following the correct syntax.", token, self.current_token.token_type);
            exit(0)
        }
    }

    pub fn get_current_token_and_expect(&mut self, token: Tokens) -> Box<Token> {
        if self.current_token.token_type == token || self.current_token.ident == token {
            let token = self.current_token.clone();
            self.parse();
            return token
        }

        println!("Expected token \x1b[91m{:?}\x1b[0m, but got \x1b[92m{:?}\x1b[0m \nPlease make sure you are following the correct syntax.", token, self.current_token.token_type);
        exit(0)
    }

    pub fn parse_function(&mut self) {
        self.expect(Tokens::IdentifierType);
        let function_name = self.get_current_token_and_expect(Tokens::Identifier).content.unwrap();
        //self.instructions.push(Instruction::new(Tokens::Identifier, format!("{}:", function_name)));
        
        self.expect(Tokens::RoundBracketOpen);

        while !self.expect_check(Tokens::RoundBracketClosed) {
            self.expect(Tokens::IdentifierType);
            let parameter_name = self.get_current_token_and_expect(Tokens::Identifier).content.unwrap();
            if !self.expect_check(Tokens::Comma) { 
                self.expect(Tokens::RoundBracketClosed); 
                break 
            }
        }

        self.parse_body();
    }

    fn parse_body(&mut self) {
        self.expect(Tokens::CurlyBracketOpen);
        self.depth += 1;

        while self.current_token.token_type != Tokens::CurlyBracketClosed {
            if self.current_token.token_type == Tokens::CurlyBracketOpen {
                self.parse_body()
            } else if self.current_token.token_type == Tokens::For {
                self.parse();
                self.parse_for_loop()
            } else if self.current_token.token_type == Tokens::If {
                self.parse();
                self.parse_if_statement()
            } else if self.current_token.ident == Tokens::IdentifierType {
                self.parse_variable()
            }
            // Parse next token
            self.parse();
        }

        self.depth -= 1;
    }

    fn parse_for_loop(&mut self) {
        self.expect(Tokens::RoundBracketOpen);

        self.parse_variable();
        self.instructions.push(Instruction::new(Tokens::Invalid, format!("JMP .L{}", self.depth + 10)));
        self.instructions.push(Instruction::new(Tokens::Invalid, format!(".L{}:", self.depth)));
        
        self.expect(Tokens::Semicolon);
        let variable = self.get_current_token_and_expect(Tokens::Identifier);
        let variable_name = variable.content.unwrap();
        let variable_type = variable.token_type;
        let condition = self.parse_condition(&variable_name);
        self.parse(); // Skip condition
        self.expect(Tokens::Semicolon);

        self.parse_expression(&variable_name, &variable_type);
        
        self.expect(Tokens::Semicolon);
        self.expect(Tokens::RoundBracketClosed);

        self.parse_body(); // Parse content inside the new body

        self.instructions.push(Instruction::new(Tokens::Invalid, format!(".L{}:", self.depth + 10)));
        self.instructions.push(Instruction::new(Tokens::Invalid, format!("{}", condition.0)));
        self.instructions.push(Instruction::new(Tokens::Invalid, format!("{}", condition.1)));

        //self.advance(Tokens::CurlyBracketClosed);
    }

    fn parse_if_statement(&mut self) {

    }

    fn parse_condition(&mut self, variable_name: &str) -> (String, String) {
        let condition = match self.current_token.content.clone().unwrap().as_str() {
            "<" => "JL",
            "<=" => "JLE",
            "==" => "JE",
            "!=" => "JNE",
            ">" => "JG",
            ">=" => "JGE",
            _ => "Invalid",
        };

        self.parse();

        (format!("CMP {}, {}", variable_name, self.current_token.content.clone().unwrap()), format!("{} .L{}", condition, self.depth))
    }

    fn parse_variable(&mut self) {
        let variable_type = self.get_current_token_and_expect(Tokens::IdentifierType).token_type;
        let variable_name = self.get_current_token_and_expect(Tokens::Identifier).content.unwrap();
        self.expect(Tokens::OperatorAssign);
        self.parse_statement(&variable_name, &variable_type);
    }

    fn parse_statement(&mut self, variable_name: &str, variable_type: &Tokens) {
        let value = self.current_token.content.clone().unwrap();

        let intermediate = format!("ASSIGN {}, {}", variable_name, value);
        self.instructions.push(Instruction::new(variable_type.clone(), intermediate));

        if !self.expect_check(Tokens::Semicolon) {
            self.parse_expression(variable_name, variable_type)
        }
    }

    fn parse_expression(&mut self, variable_name: &str, variable_type: &Tokens) {
        while self.current_token.token_type != Tokens::Semicolon {
            match self.current_token.token_type {
                Tokens::OperatorPlus => self.parse_operation("ADD", variable_name, variable_type),
                Tokens::OperatorMinus => self.parse_operation("SUB", variable_name, variable_type),
                Tokens::OperatorMultiply => self.parse_operation("MUL", variable_name, variable_type),
                Tokens::OperatorDivision => self.parse_operation("DIV", variable_name, variable_type),
                Tokens::OperatorModulo => self.parse_operation("MOD", variable_name, variable_type),
                Tokens::OperatorBitwiseAnd => self.parse_operation("AND", variable_name, variable_type),
                Tokens::OperatorBitwiseOr => self.parse_operation("OR", variable_name, variable_type),
                Tokens::OperatorBitwiseXor => self.parse_operation("XOR", variable_name, variable_type),
                Tokens::OperatorBitwiseNot => self.parse_bitwise_not(variable_name),
                Tokens::OperatorBitwiseSHL => self.parse_operation("SHL", variable_name, variable_type),
                Tokens::OperatorBitwiseSHR => self.parse_operation("SHR", variable_name, variable_type),
                _ => { }
            }

            self.parse()
        }
    }

    fn parse_operation(&mut self, operation_intermediate: &str, variable_name: &str, variable_type: &Tokens) {
        self.parse();
        let operand_2 = self.current_token.content.clone().unwrap();
        let intermediate = format!("{} {}, {}", operation_intermediate, variable_name, operand_2);
        
        self.instructions.push(Instruction::new(variable_type.clone(), intermediate))
    }

    #[allow(unused)]
    fn parse_bitwise_not(&mut self, variable_name: &str) {
        todo!()
    }
}