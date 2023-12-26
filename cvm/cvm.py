import assembler

code = """
add:
    ARRAY my_array, 4, i64
    VAR i, i64
    MOV i, 0
    JMP .L1
.L0:
    MOV my_array[i], i
    ADD i, 1
.L1:
    CMP i, 4
    jne, .L0
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