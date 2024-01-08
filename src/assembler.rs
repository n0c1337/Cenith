use crate::{instruction::Instruction, tokens::{get_token_size, get_token_stack_size}};

pub struct Assembler {
    instructions: Vec<Instruction>,
    variables: Vec<(String, String, i32, i32)>
}

impl Assembler {
    pub fn new(instruction_stream: Vec<Instruction>) -> Self {
        Assembler { instructions: instruction_stream, variables: Vec::new() }
    }

    pub fn translate(&mut self) {
        self.translate_function();

        for i in &self.variables {
            println!("INFO: {} {} {} {}", i.0, i.1, i.2, i.3)
        }
    }

    fn find_variables(&mut self) {
        for i in self.instructions.iter() {
            if i.value.contains("ASSIGN") {
                let split: Vec<&str> = i.value.split_whitespace().collect();
                let variable = split.get(1).unwrap();
                for j in &self.variables {
                    if *j.0 == variable.to_string() {
                        break;
                    }
                }

                let size = get_token_size(i.token_type.clone());
                self.variables.push((variable.to_string().replace(",", ""), get_token_stack_size(size).to_string(), size, size));
            }
        }
    }

    fn obtain_stack_size(&mut self) -> i32 {
        let mut stack_size = 0;
        for i in self.variables.iter_mut() {
            stack_size += i.3;
            i.3 = stack_size;
        }

        stack_size
    }

    fn translate_function(&mut self) {
        self.find_variables();
        let stack_size = self.obtain_stack_size();

        let mut assembly: Vec<String> = Vec::new();

        // Prologue
        assembly.push("push rbp".to_string());
        assembly.push("mov rbp, rsp".to_string());
        // Reserve space for local variables
        assembly.push(format!("sub rsp, {}", stack_size));

        for i in self.instructions.iter() {
            let asm = self.generate_assembly(&i.value);
            assembly.push(asm);
        }

        // Epilogue
        // Free local variables
        assembly.push(format!("add rsp, {}", stack_size));
        assembly.push("mov rsp, rbp".to_string());
        assembly.push("pop rbp".to_string());
        
        for i in assembly {
            println!("{}", i)
        }
    }

    // This function will generate x86-64 NASM Assembly code. 
    fn generate_assembly(&self, instruction: &str) -> String {
        let mut assembly = String::new();

        // Check for labels
        if instruction.starts_with(".") {
            assembly.push_str(instruction);
            return assembly
        }

        let mut operand_1 = String::new();
        let mut operand_2 = String::new();
        
        let operation = String::from(instruction);
        let split: Vec<&str> = operation.split_whitespace().collect();
        let intermediate = split.get(0).unwrap();
        let first_operand = split.get(1).unwrap_or(&"").replace(",", "");
        let second_operand = split.get(2).unwrap_or(&"").replace(",", "");

        operand_1 = first_operand.clone();
        operand_2 = second_operand.clone();
        let mut register = "r10";
        for i in &self.variables {
            if i.0 == first_operand {
                operand_1 = format!("{} [rsp-{}]", i.1, i.3);
                match i.2 {
                    1 => register = "r10b",
                    2 => register = "r10w",
                    4 => register = "r10d",
                    8 => register = "r10",
                    _ => { }
                }
            } 
            if i.0 == second_operand {
                operand_2 = format!("{} [rsp-{}]", i.1, i.3)
            }
        }

        

        if !operation.contains(".") { assembly.push_str(&format!("mov {}, {}\n", register, operand_1)) }
        match *intermediate {
            "ADD" => assembly.push_str(&self.generate_operation("add", register, &operand_2)),
            "SUB" => assembly.push_str(&self.generate_operation("sub", register, &operand_2)),
            "MUL" => assembly.push_str(&self.generate_operation("imul", register, &operand_2)),
            "DIV" => assembly.push_str(&self.generate_operation("idiv", register, &operand_2)),
            "MOD" => assembly.push_str(&self.generate_operation("idiv", register, &operand_2)),
            "ASSIGN" => assembly.push_str(&self.generate_operation("mov", register, &operand_2)),
            "AND" => assembly.push_str(&self.generate_operation("and", register, &operand_2)),
            "OR" => assembly.push_str(&self.generate_operation("or", register, &operand_2)),
            "XOR" => assembly.push_str(&self.generate_operation("xor", register, &operand_2)),
            "NOT" => todo!(),
            "SHL" => assembly.push_str(&self.generate_operation("shl", register, &operand_2)),
            "SHR" => assembly.push_str(&self.generate_operation("shr", register, &operand_2)),
            "CMP" => assembly.push_str(&self.generate_operation("cmp", register, &operand_2)),
            "JMP" => assembly.push_str(&self.generate_operation("jmp", &operand_1, &operand_2)),
            "JL" => assembly.push_str(&self.generate_operation("jl", &operand_1, &operand_2)),
            "JLE" => assembly.push_str(&self.generate_operation("jle", &operand_1, &operand_2)),
            "JE" => assembly.push_str(&self.generate_operation("je", &operand_1, &operand_2)),
            "JNE" => assembly.push_str(&self.generate_operation("jne", &operand_1, &operand_2)),
            "JG" => assembly.push_str(&self.generate_operation("jg", &operand_1, &operand_2)),
            "JGE" => assembly.push_str(&self.generate_operation("jge", &operand_1, &operand_2)),
            _ => assembly.push_str("ERROR\n"),
        }
        if !operation.contains(".") { assembly.push_str(&format!("mov {}, {}\n", operand_1, register)) }

        assembly
    }

    fn generate_operation(&self, instruction: &str, operand_1: &str, operand_2: &str) -> String {
        format!("{} {}, {}\n", instruction, operand_1, operand_2)
    } 
}