use crate::tokens::Tokens;

#[derive(Clone, Debug)]
pub struct Instruction {
    pub token_type: Tokens,
    pub value: String,
}

impl Instruction {
    pub fn new(token_type: Tokens, value: String) -> Self {
        Instruction { token_type: token_type, value: value }
    }
}