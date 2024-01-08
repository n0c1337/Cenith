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
        let to_be_parsed = fs::read_to_string("./tests/test.cenith")
            .expect("Couldn't read file.");

        let mut character_iterator = to_be_parsed.chars().skip(self.start_index);
        
        while let Some(c) = character_iterator.next() {
            if !c.is_whitespace() {
                break;
            }

            self.start_index += 1;
        }

        let mut end_index = self.start_index;

        while let Some(c) = character_iterator.next() {
            if c.is_whitespace() || is_delimiter_character(c) {
                break;
            }

            end_index += 1;
        }

        if end_index >= to_be_parsed.len() {
            return;
        }
        
        println!("{} -> {}", self.start_index, end_index);
        println!("|{}|", &to_be_parsed[self.start_index..=end_index]);

        let token = Box::new(Lexer::new().lex(&to_be_parsed[self.start_index..=end_index]));
        *self.current_token = *token;
        println!("{:?}", *self.current_token);

        self.start_index = end_index+1;
    }

    pub fn advance_check(&mut self, token: Tokens) -> bool {
        if self.current_token.token_type == token || self.current_token.ident == token {
            self.parse();
            true
        } else {
            false
        }
    }

    pub fn advance(&mut self, token: Tokens) {
        if !self.advance_check(token.clone()) {
            println!("Expected token \x1b[91m{:?}\x1b[0m, but got \x1b[92m{:?}\x1b[0m \nPlease make sure you are following the correct syntax.", token, self.current_token.token_type);
            exit(0)
        }
    }

    pub fn parse_function(&mut self) {
        self.advance(Tokens::IdentifierType);
        self.advance(Tokens::Identifier);
        self.advance(Tokens::RoundBracketOpen);

        while self.current_token.token_type != Tokens::RoundBracketClosed {
            self.advance(Tokens::IdentifierType);
            self.advance(Tokens::Identifier);
            if self.advance_check(Tokens::Commata) { /* Skip */ }
        }

        self.advance(Tokens::RoundBracketClosed);

        self.parse_body();
    }

    fn parse_body(&mut self) {
        self.advance(Tokens::CurlyBracketOpen);
        self.depth += 1;

        {
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
        }

        //self.advance(Tokens::CurlyBracketClosed); This will cause the very first token after the closed curly bracket to be loss
        self.depth -= 1;
    }

    fn parse_for_loop(&mut self) {
        self.advance(Tokens::RoundBracketOpen);

        self.parse_variable();
        self.instructions.push(Instruction::new(Tokens::Invalid, format!("JMP .L{}", self.depth + 10)));
        self.instructions.push(Instruction::new(Tokens::Invalid, format!(".L{}:", self.depth)));
        
        /*
            We are going to skip some tokens cause we handle them a bit different then before.
         */
        self.parse(); // Skip Semicolon
        let variable_name = self.current_token.content.clone().unwrap();
        self.parse(); // Skip variable identifier
        let condition = self.parse_condition(variable_name.as_str());
        self.parse(); // Skip condition
        self.advance(Tokens::Semicolon);

        // TODO: resolve variable type just for having an identical convergence
        self.parse_expression(&Tokens::Invalid, variable_name.as_str());
        
        self.parse(); // Skip Semicolon
        self.advance(Tokens::RoundBracketClosed);

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
        let variable_type = self.current_token.token_type.clone();
        self.parse();
        let variable_name = self.current_token.content.clone().unwrap();
        self.advance(Tokens::Identifier);
        self.advance(Tokens::OperatorAssign);

        self.parse_statement(variable_name.as_str(), &variable_type);
    }

    fn parse_statement(&mut self, variable_name: &str, variable_type: &Tokens) {
        let value = self.current_token.content.clone().unwrap();

        let intermediate = format!("ASSIGN {}, {}", variable_name, value);
        self.instructions.push(Instruction::new(variable_type.clone(), intermediate));

        if !self.advance_check(Tokens::Semicolon) {
            self.parse_expression(variable_type, variable_name)
        }
    }

    fn parse_expression(&mut self, variable_type: &Tokens, variable_name: &str) {
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
        let value = self.current_token.content.clone().unwrap();
        let intermediate = format!("{} {}, {}", operation_intermediate, variable_name, value);
        
        self.instructions.push(Instruction::new(variable_type.clone(), intermediate))
    }

    #[allow(unused)]
    fn parse_bitwise_not(&mut self, variable_name: &str) {
        todo!()
    }
}