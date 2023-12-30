import assembler

#code = """
#add:
#    ARRAY my_array, 4, i64
#    VAR i, i64
#    MOV i, 0
#    JMP .L1
#.L0:
#    MOV my_array[i], i
#    ADD i, 1
#.L1:
#    CMP i, 4
#    jne, .L0
#"""

code = """
main:
    ASSIGN helloworld, 1
    ASSIGN i, 2
.L2:
    ADD i, 1
    ASSIGN a, 10
.L3:
    SUB a, 1
    ADD i, 1
    CMP a, 5
    JG .L3
    ADD i, 1
    CMP i, 99
    JNE .L2
    RET 0
"""

variables = []
sections = {
    ".data": [],
    ".text": []
}
ir = assembler.Assembler(sections=sections, variables=variables)

for line in code.splitlines():
    if line != "":
        ir.translate_ir_to_assembly(line.replace(',', ''))

print(".data")
for i in sections[".data"]:
    print(i)

print(".text")
for i in sections[".text"]:
    print(i)