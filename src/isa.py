import json
from enum import Enum


class ArgumentTypes(str, Enum):
    Direct = "DirectArgument",
    Indirect = "IndirectArgument"

    def __repr__(self) -> str:
        return super().__repr__()


class Opcode(str, Enum):
    INC = "inc"
    DEC = "dec"
    ITOC = "itoc"
    CTOI = "ctoi"
    ADD_M = "add_m"
    ADD_IMM = "add_imm"
    SUB_M = "sub_m"
    SUB_IMM = "sub_imm"
    DIV_M = "div_m"
    DIV_IMM = "div_imm"
    MOD_M = "mod_m"
    MOD_IMM = "mod_imm"
    MUL_M = "mul_m"
    MUL_IMM = "mul_imm"
    LD_M = "ld_m"
    LD_IMM = "ld_imm"
    ST_IMM = "st_imm"
    CMP_M = "cmp_m"
    CMP_IMM = "cmp_imm"
    JE = "je"
    JNE = "jne"
    JS = "js"
    JMP = "jmp"
    IN_IMM = "in_imm"
    OUT_IMM = "out_imm"
    HALT = "halt"


class Command:
    def __init__(self, name, in_vars=None):
        if in_vars is None:
            in_vars = []

        self.vars = in_vars
        self.name = name

    def add_variation(self, opcode: Opcode, args: list[ArgumentTypes]):
        self.vars.append((opcode, args))

    def get_opcode(self, in_args: list[ArgumentTypes]):
        for (opcode, args) in self.vars:
            if args == in_args:
                return opcode
        return None

    def get_command_vars(self):
        return [var[1] for var in self.vars]

    def __repr__(self) -> str:
        return str({"name": self.name, "vars": self.vars})


# Command dictionary
class CommandDict:
    def __init__(self):
        self.dct: dict[str, Command] = {}

    def append(self, cmd: Command) -> None:
        name = cmd.name
        if name in self.dct:
            raise Exception(f"Already has command '{name}'")
        else:
            self.dct[name] = cmd

    def get_command(self, name: str):
        if name in self.dct:
            return self.dct[name]
        else:
            return None

    def __repr__(self) -> str:
        return str(self.dct)


# Init with commands
def init_commands(commands: CommandDict):
    commands.append(Command("inc", [
        (Opcode.INC, [])
    ]))
    commands.append(Command("dec", [
        (Opcode.DEC, [])
    ]))

    commands.append(Command("itoc", [
        (Opcode.ITOC, [])
    ]))

    commands.append(Command("ctoi", [
        (Opcode.CTOI, [])
    ]))

    commands.append(Command("add", [
        (Opcode.ADD_M, [ArgumentTypes.Indirect]),
        (Opcode.ADD_IMM, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("sub", [
        (Opcode.SUB_M, [ArgumentTypes.Indirect]),
        (Opcode.SUB_IMM, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("div", [
        (Opcode.DIV_M, [ArgumentTypes.Indirect]),
        (Opcode.DIV_IMM, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("mod", [
        (Opcode.MOD_M, [ArgumentTypes.Indirect]),
        (Opcode.MOD_IMM, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("mul", [
        (Opcode.MUL_M, [ArgumentTypes.Indirect]),
        (Opcode.MUL_IMM, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("ld", [
        (Opcode.LD_M, [ArgumentTypes.Indirect]),
        (Opcode.LD_IMM, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("st", [
        (Opcode.ST_IMM, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("cmp", [
        (Opcode.CMP_M, [ArgumentTypes.Indirect]),
        (Opcode.CMP_IMM, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("je", [
        (Opcode.JE, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("jne", [
        (Opcode.JNE, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("js", [
        (Opcode.JS, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("jmp", [
        (Opcode.JMP, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("in", [
        (Opcode.IN_IMM, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("out", [
        (Opcode.OUT_IMM, [ArgumentTypes.Direct]),
    ]))

    commands.append(Command("halt", [
        (Opcode.HALT, []),
    ]))


# Init commands
ISACommands = CommandDict()
init_commands(ISACommands)


# Read and write
def write_code(fname, code):
    with open(fname, "w", encoding="utf-8") as file:
        file.write(json.dumps(code, indent=2))


def read_code(fname):
    with open(fname, encoding="utf-8") as file:
        code = json.loads(file.read())

    for inst in code:
        # Конвертация строки в Opcode
        if "opcode" in inst:
         inst["opcode"] = Opcode(inst['opcode'])
    return code
