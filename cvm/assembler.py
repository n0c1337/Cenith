class Assembler:
    def __init__(self, sections: dict, variables: list) -> None:
        self.variables = variables
        self.sections = sections
    
    def translate_ir_to_assembly(self, line: str):
        statement = line.split()
        instruction = statement[0]
        assembly: str = None
        match instruction:
            case "extern":
                extern = statement[1]
                assembly = f"extern {extern}"
            case "NEW_STACK_FRAME":
                assembly = f"push rbp\nmov rbp, rsp"
                pass
            case "DELETE_STACK_FRAME":
                assembly = f"mov rsp, rbp\npop rbp" # Alternativly "leave" instruction
                pass
            case "GVAR":
                variable_name = statement[1]
                variable_type = statement[2]
            case "VAR": 
                variable_name = statement[1]
                variable_type = statement[2]
                size = 0
                match variable_type:
                    case "i8":
                        size = 1
                    case "i16":
                        size = 2
                    case "i32":
                        size = 4
                    case "i64":
                        size = 8
                assembly = f"sub rsp, {size}"
                self.variables.append(f"{variable_name}:{variable_type}")
            case "STRING":
                string_name = statement[1]
                string_content = statement[2]
                self.sections[".data"].append(f"{string_name} DB {string_content}, 0")
                self.sections[".data"].append(f"{string_name}_len equ $-{string_name}")
            case "MOV":
                destination = statement[1]
                source = statement[2]
                assembly = f"mov {destination}, {source}"
            case "CMP":
                comperand1 = statement[1]
                comperand2 = statement[2]
                assembly = f"cmp {comperand1}, {comperand2}"
            case "JL":
                address = statement[1]
                assembly = f"jl {address}"
            case "JLE":
                address = statement[1]
                assembly = f"jle {address}"
            case "JE":
                address = statement[1]
                assembly = f"je {address}"
            case "JNE":
                address = statement[1]
                assembly = f"jne {address}"
            case "JG":
                address = statement[1]
                assembly = f"jg {address}"
            case "JGE":
                address = statement[1]
                assembly = f"jge {address}"
            case "JMP":
                address = statement[1]
                assembly = f"jmp {address}"
            case "JZ":
                address = statement[1]
                assembly = f"jz {address}"
            case "JNZ":
                address = statement[1]
                assembly = f"jnz {address}"
            case "TEST":
                comperand = statement[1]
                assembly = f"test {comperand}, {comperand}"
            case "ARRAY":
                array_name = statement[1]
                array_size = statement[2]
                array_type = statement[3]
                define  = None
                size = 0
                match array_type:
                    case "i8":
                        define = "DB"
                        size = 1
                    case "i16":
                        define = "DW"
                        size = 2
                    case "i32":
                        define = "DD"
                        size = 4
                    case "i64":
                        define = "DQ"
                        size = 8
                actual_size = size * int(array_size)
                self.sections[".data"].append(f"{array_name} TIMES {actual_size} {define} 0")
            case "CALL":
                function_name = statement[1]
                assembly = f"call {function_name}"
            case "RET":
                return_value = statement[1]
                # TODO: Check if return value is int, float, i32, whatever 
                # TODO: Check size to use the right register e.g eax for i32 and rax for i64
                assembly = "ret" if return_value is None else f"mov rax, {return_value}\nret"
            case "ADD":
                destination = statement[1]
                source = statement[2]
                assembly = f"add {destination}, {source}"
            case "SUB":
                destination = statement[1]
                source = statement[2]
                assembly = f"sub {destination}, {source}"
            case "MUL":
                destination = statement[1]
                source = statement[2]
                assembly = f"imul {destination}, {source}"
            case "DIV":
                destination = statement[1]
                source = statement[2]
                assembly = f"idiv {destination}, {source}"
            case "AND":
                destination = statement[1]
                source = statement[2]
                assembly = f"and {destination}, {source}"
            case "OR":
                destination = statement[1]
                source = statement[2]
                assembly = f"or {destination}, {source}"
            case "XOR":
                destination = statement[1]
                source = statement[2]
                assembly = f"xor {destination}, {source}"
            case "NOT":
                destination = statement[1]
                assembly = f"not {destination}"
            case "SHL":
                destination = statement[1]
                source = statement[2]
                assembly = f"shl {destination}, {source}"
            case "SHR":
                destination = statement[1]
                source = statement[2]
                assembly = f"shr {destination}, {source}"

        if instruction.endswith(":"):
            assembly = f"{instruction}"
        elif instruction.startswith("."):
            assembly = f"{instruction}"
        
        if assembly is not None:
            self.sections[".text"].append(assembly)