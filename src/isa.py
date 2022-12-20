import json
from collections import namedtuple
from enum import Enum

# Опкод выглядит не с точки зрения машинного кода
class Opcode(str, Enum):
    INC = "increment"
    DEC = "decrement"

    ITOC = "integer_to_char"
    CTOI = "char_to_integer"

    ADD_M = "add_m32"
    ADD_IMM = "add_imm32"

    SUB_M = "sub_m32"
    SUM_IMM = "sub_imm32"

    DIV_M= "div_m32"
    DIV_IMM = "div_imm32"

    MOD_M = "mod_m32"
    MOD_IMM = "mod_imm32"

    MUL_M = "mul_m32"
    MUL_IMM = "mul_imm32"

    LD_M = "ld_m32"
    LD_IMM = "ld_imm32"

    ST_IMM = "st_imm32"

    CMP_M = "cmp_m32"
    CMP_IMM = "cmp_imm32"

    JE_IMM = "jmp_if_eq"
    JNE_IMM = "jmp_if_neq"
    JS_IMM = "jmp_if_neg"
    JMP_IMM = "jmp"

    IN = "in_port"
    OUT = "out_port"

    HALT = "halt"

class Term(namedtuple('Term', 'line pos symbol')):
    pass


def write_code(fname, code):
    with open(fname, "w", encoding="utf-8") as file:
        file.write(json.dumps(code, indent=4))


def read_code(filename):
    with open(filename, encoding="utf-8") as file:
        code = json.loads(file.read())

    for instr in code:
        # Конвертация строки в Opcode
        instr['opcode'] = Opcode(instr['opcode'])

        # Конвертация списка из term в класс Term
        if 'term' in instr:
            instr['term'] = Term(
                instr['term'][0], instr['term'][1], instr['term'][2])

    return code
