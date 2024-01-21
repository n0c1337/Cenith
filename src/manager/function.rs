use crate::{instruction::Instruction, tokens::{get_token_size, get_token_stack_size, get_right_sized_register, LEA_REGISTER}};

pub struct FunctionManager {
    name: String,
    parameter: Option<Vec<String>>,
    variables: Vec<(String, String, i32, i32)>,
}

impl FunctionManager {
    pub fn new(name: &str) -> Self {
        FunctionManager { name: name.to_string(), parameter: None, variables: Vec::new() }
    }

    fn find_variables(&mut self, instructions: &Vec<Instruction>) {
        for i in instructions {
            if i.content.contains("ASSIGN") {
                let split: Vec<&str> = i.content.split_whitespace().collect();
                let variable = split.get(1).unwrap().to_string().replace(",", "");
                for j in &self.variables {
                    if *j.0 == variable {
                        return;
                    }
                }

                let size = get_token_size(i.token_type.clone());
                self.variables.push((variable, get_token_stack_size(size).to_string(), size, size))
            }
        }
    }

    fn calculate_stack_size(&mut self) -> i32 {
        let mut stack_size = 0;
        for i in &mut self.variables {
            stack_size += i.3;
            i.3 = stack_size;
        }

        stack_size
    }

    fn find_variable(&self, variable_name: &str) -> String {
        for i in &self.variables {
            if i.0 == variable_name {
                let assembly = format!("{} [rsp-{}]", i.1, i.3);
                return assembly 
            }
        }

        String::from("Invalid")
    }
    
    fn generate_assembly(&self, instruction: &str) -> String {
        let mut assembly = String::new();

        if instruction.starts_with(".") || instruction.ends_with(":") {
            assembly.push_str(instruction);
            return assembly
        }

        let intermediate = String::from(instruction);
        let split: Vec<&str> = intermediate.split_whitespace().collect();
        let mnemonic = split.get(0).unwrap();
        let mut first_operand = split.get(1).unwrap_or(&"Invalid").replace(",", "");
        let mut second_operand = split.get(2).unwrap_or(&"Invalid").replace(",", "");

        let mut register = "r10";
        for i in &self.variables {
            if i.0 == first_operand {
                first_operand = format!("{} [rsp-{}]", i.1, i.3);
                register = get_right_sized_register(i.2)
            }

            if i.0 == second_operand {
                second_operand = format!("{} [rsp-{}]", i.1, i.3)
            } 
        }

        if *mnemonic == "ASSIGN" {
            if second_operand.starts_with("&") {
                assembly.push_str(&format!("lea {}, {}\n", LEA_REGISTER, self.find_variable(&second_operand.replace("&", ""))));
                assembly.push_str(&format!("mov {}, {}\n", first_operand, LEA_REGISTER));
            } else {
                assembly.push_str(&format!("mov {}, {}\n", register, second_operand));
                assembly.push_str(&format!("mov {}, {}\n", first_operand, register));
            }
            return assembly
        }

        if !intermediate.contains(".") { assembly.push_str(&format!("mov {}, {}\n", register, first_operand)) }
        match *mnemonic {
            "ADD" => assembly.push_str(&self.generate_operation("add", register, &second_operand)),
            "SUB" => assembly.push_str(&self.generate_operation("sub", register, &second_operand)),
            "MUL" => assembly.push_str(&self.generate_operation("imul", register, &second_operand)),
            "DIV" => assembly.push_str(&self.generate_operation("idiv", register, &second_operand)),
            "MOD" => assembly.push_str(&self.generate_operation("idiv", register, &second_operand)),
            "ASSIGN" => assembly.push_str(&self.generate_operation("mov", register, &second_operand)),
            "AND" => assembly.push_str(&self.generate_operation("and", register, &second_operand)),
            "OR" => assembly.push_str(&self.generate_operation("or", register, &second_operand)),
            "XOR" => assembly.push_str(&self.generate_operation("xor", register, &second_operand)),
            "NOT" => todo!(),
            "SHL" => assembly.push_str(&self.generate_operation("shl", register, &second_operand)),
            "SHR" => assembly.push_str(&self.generate_operation("shr", register, &second_operand)),
            "CMP" => assembly.push_str(&self.generate_operation("cmp", register, &second_operand)),
            "JMP" => assembly.push_str(&self.generate_operation("jmp", &first_operand, &second_operand)),
            "JL" => assembly.push_str(&self.generate_operation("jl", &first_operand, &second_operand)),
            "JLE" => assembly.push_str(&self.generate_operation("jle", &first_operand, &second_operand)),
            "JE" => assembly.push_str(&self.generate_operation("je", &first_operand, &second_operand)),
            "JNE" => assembly.push_str(&self.generate_operation("jne", &first_operand, &second_operand)),
            "JG" => assembly.push_str(&self.generate_operation("jg", &first_operand, &second_operand)),
            "JGE" => assembly.push_str(&self.generate_operation("jge", &first_operand, &second_operand)),
            _ => assembly.push_str("; Invalid Mnemonic\n"),
        }
        if !intermediate.contains(".") { assembly.push_str(&format!("mov {}, {}\n", first_operand, register)) }

        assembly
    }

    fn generate_operation(&self, instruction: &str, operand_1: &str, operand_2: &str) -> String {
        format!("{} {}, {}\n", instruction, operand_1, operand_2)
    } 

    pub fn translate(&mut self, instructions: &Vec<Instruction>) -> Vec<String> {
        self.find_variables(instructions);
        let stack_size = self.calculate_stack_size();
        let mut assembly: Vec<String> = Vec::new();

        assembly.push(format!("{}:", self.name));
        assembly.push(String::from("push rbp"));
        assembly.push(String::from("mov rbp, rsp"));
        assembly.push(format!("sub rsp, {}", stack_size));

        for i in instructions {
            assembly.push(self.generate_assembly(&i.content))
        }

        assembly.push(format!("add rsp, {}", stack_size));
        assembly.push(String::from("mov rsp, rbp"));
        assembly.push(String::from("pop rbp"));

        assembly
    }
}