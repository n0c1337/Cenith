use crate::tokens::Tokens;

#[derive(Clone, Debug)]
pub struct Instruction {
    pub token_type: Tokens,
    pub content: String,
}

impl Instruction {
    pub fn new(token_type: Tokens, content: String) -> Self {
        Instruction { token_type: token_type, content: content }
    }
}