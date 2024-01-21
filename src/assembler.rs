use crate::{instruction::Instruction, manager::function::FunctionManager};

pub struct Assembler {
    instructions: Vec<Instruction>,
}

impl Assembler {
    pub fn new(instruction_stream: Vec<Instruction>) -> Self {
        Assembler { instructions: instruction_stream }
    }

    pub fn translate(&mut self) {
        let mut function_manager = FunctionManager::new("test");
        let assembly_code = function_manager.translate(&self.instructions);

        for i in &assembly_code {
            println!("{}", i)
        }
    }
}