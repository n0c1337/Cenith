use assembler::Assembler;

use crate::parser::Parser;

mod assembler;
mod instruction;
mod lexer;
mod parser;
mod tokens;

fn main() {
    let mut parser = Parser::new();

    parser.parse();
    parser.parse_function();

    for i in &parser.instructions {
        println!("{:?}", *i)
    }

    let mut assembler = Assembler::new(parser.instructions);
    assembler.translate()
}
