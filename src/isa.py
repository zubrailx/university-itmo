import json
from collections import namedtuple
from enum import Enum

# Опкод выглядит не с точки зрения машинного кода
class Opcode(str, Enum):
    INC = "increment"
    DEC = "decrement"

    ITOC = "integer_to_char"
    CTOI = "char_to_integer"

    ADD_M = "add_m24"
    ADD_IMM = "add_imm24"

    SUB_M = "sub_m24"
    SUM_IMM = "sub_imm24"

    DIV_M= "div_m24"
    DIV_IMM = "div_imm24"

    MOD_M = "mod_m24"
    MOD_IMM = "mod_imm24"

    MUL_M = "mul_m24"
    MUL_IMM = "mul_imm24"

    LD_M = "ld_m24"
    LD_IMM = "ld_imm24"

    ST_IMM = "st_imm24"

    CMP_M = "cmp_m24"
    CMP_IMM = "cmp_imm24"

    JE_IMM = "jmp_if_eq"
    JNE_IMM = "jmp_if_neq"
    JS_IMM = "jmp_if_neg"
    JMP_IMM = "jmp"

    IN = "in_port"
    OUT = "out_port"

    HALT = "halt"

# class Term(namedtuple('Term', 'line pos symbol')):
#     pass


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
        # if 'term' in instr:
        #     instr['term'] = Term(
        #         instr['term'][0], instr['term'][1], instr['term'][2])

    return code
