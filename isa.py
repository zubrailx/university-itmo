# pylint: disable=missing-function-docstring  # чтобы не быть Капитаном Очевидностью
# pylint: disable=invalid-name                # сохраним традиционные наименования сигналов
# pylint: disable=consider-using-f-string     # избыточный синтаксис
# pylint: disable=too-few-public-methods

"""
Типы данных для представления и сериализации/десериализации машинного кода.
"""

import json
from enum import Enum
from typing import Optional, Any

WORD_WIDTH = 32
WORD_MAX_VALUE = 2 ** (WORD_WIDTH - 1) - 1
WORD_MIN_VALUE = 2 ** (WORD_WIDTH - 1) * (-1)


class ArgumentTypes(str, Enum):
    """Типы аргументов, передаваемые ассемблеру."""

    Direct = "DirectArgument"
    Indirect = "IndirectArgument"


class Opcode(str, Enum):
    """Opcode для ISA."""

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
    JB = "jb"  # below
    JG = "jg"  # greater
    JBE = "jbe"  # below or equals
    JGE = "jge"  # greater or equals
    JMP = "jmp"
    IN_IMM = "in_imm"
    OUT_IMM = "out_imm"
    HALT = "halt"

    def __repr__(self) -> str:
        return str(self.value)


OpcodeCommand = tuple[Opcode, tuple[ArgumentTypes, ...]]


class Command:
    """Команда и ее вариации с аргументами."""

    def __init__(self, name: str, in_vars: Optional[list[OpcodeCommand]] = None) -> None:
        if in_vars is None:
            in_vars = []

        self.vars: list[tuple[Opcode, tuple[ArgumentTypes, ...]]] = in_vars
        self.name: str = name

    def add_variation(self, opcode: Opcode, args: tuple[ArgumentTypes]) -> None:
        self.vars.append((opcode, args))

    def get_opcode(self, in_args: tuple[ArgumentTypes, ...]) -> Optional[Opcode]:
        for (opcode, args) in self.vars:
            if args == in_args:
                return opcode
        return None

    def get_command_vars(self) -> list[tuple[ArgumentTypes, ...]]:
        return [var[1] for var in self.vars]

    def __repr__(self) -> str:
        return str({"name": self.name, "vars": self.vars})


class CommandDict:
    """Словарь команд ISA, с которыми работает machine.py."""

    def __init__(self) -> None:
        self.dct: dict[str, Command] = {}

    def append(self, cmd: Command) -> None:
        name = cmd.name
        if name not in self.dct:
            self.dct[name] = cmd
        else:
            raise Exception(f"ISA Command dictionary already has command '{name}'")

    def get_command(self, name: str) -> Optional[Command]:
        if name in self.dct:
            return self.dct[name]
        return None

    def __repr__(self) -> str:
        return str(self.dct)


# Init with commands
def init_commands(commands: CommandDict) -> None:
    commands.append(Command("inc", [
        (Opcode.INC, tuple([]))
    ]))
    commands.append(Command("dec", [
        (Opcode.DEC, tuple([]))
    ]))

    commands.append(Command("itoc", [
        (Opcode.ITOC, tuple([]))
    ]))

    commands.append(Command("ctoi", [
        (Opcode.CTOI, tuple([]))
    ]))

    commands.append(Command("add", [
        (Opcode.ADD_M, tuple([ArgumentTypes.Indirect])),
        (Opcode.ADD_IMM, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("sub", [
        (Opcode.SUB_M, tuple([ArgumentTypes.Indirect])),
        (Opcode.SUB_IMM, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("div", [
        (Opcode.DIV_M, tuple([ArgumentTypes.Indirect])),
        (Opcode.DIV_IMM, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("mod", [
        (Opcode.MOD_M, tuple([ArgumentTypes.Indirect])),
        (Opcode.MOD_IMM, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("mul", [
        (Opcode.MUL_M, tuple([ArgumentTypes.Indirect])),
        (Opcode.MUL_IMM, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("ld", [
        (Opcode.LD_M, tuple([ArgumentTypes.Indirect])),
        (Opcode.LD_IMM, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("st", [
        (Opcode.ST_IMM, tuple([ArgumentTypes.Indirect])),
    ]))

    commands.append(Command("cmp", [
        (Opcode.CMP_M, tuple([ArgumentTypes.Indirect])),
        (Opcode.CMP_IMM, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("je", [
        (Opcode.JE, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("jne", [
        (Opcode.JNE, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("jb", [
        (Opcode.JB, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("jbe", [
        (Opcode.JBE, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("jg", [
        (Opcode.JG, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("jge", [
        (Opcode.JGE, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("jmp", [
        (Opcode.JMP, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("in", [
        (Opcode.IN_IMM, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("out", [
        (Opcode.OUT_IMM, tuple([ArgumentTypes.Direct])),
    ]))

    commands.append(Command("halt", [
        (Opcode.HALT, tuple()),
    ]))


# Init commands
ISACommands = CommandDict()
init_commands(ISACommands)


class Instruction:
    """Описание выражения из исходного текста программы."""

    def __init__(self, address: int = -1, opcode: Optional[Opcode] = None,
                 args: Optional[list[int]] = None, value: Optional[int] = None) -> None:
        self.address: int = address  # address should be not None
        self.opcode: Opcode | None = opcode
        self.args: Optional[list[int]] = args
        self.value: Optional[int] = value

    def __repr__(self) -> str:
        return self.__dict__.__repr__()

    def __getitem__(self, name: str) -> Any:
        return self.__dict__[name]

    def items(self) -> Any:
        return self.__dict__.items()


def _dict_to_instruction(dct: dict[str, Any]) -> Instruction:
    inst: Instruction = Instruction()
    for key, val in dct.items():
        if val is not None:
            if key == "opcode":
                setattr(inst, key, Opcode(val))
            else:
                setattr(inst, key, val)
    return inst


# Read and write
def write_code(fname: str, in_instructions: list[Instruction], start_pos: int) -> None:
    with open(fname, "w", encoding="utf-8") as file:
        instructions = []
        # remove None keys to read json easier
        for in_inst in in_instructions:
            inst = {}
            for k, v in in_inst.items():
                if v is not None:
                    inst[k] = v
            instructions.append(inst)

        code = {
            "instructions": instructions,
            "start_pos": start_pos
        }
        file.write(json.dumps(code, indent=2, sort_keys=True))


def read_code(fname: str) -> tuple[list[Instruction], int]:
    with open(fname, encoding="utf-8") as file:
        code = json.loads(file.read())

    for idx, dct in enumerate(code["instructions"]):
        code["instructions"][idx] = _dict_to_instruction(dct)

    return code["instructions"], code["start_pos"]


# INPUT/OUTPUT
# Convert json port numbers and string list input to int
def _ports_to_str(ports: dict[int, dict[str, list[int]]]) -> dict[str, dict[str, list[str]]]:
    ret = {}
    for pnum, pentry in ports.items():
        rentry = {}
        for k, vlist in pentry.items():
            rentry[k] = [chr(e) for e in vlist]
        ret[str(pnum)] = rentry
    return ret


# Convert json port numbers and string list input to str
def _ports_to_int(ports: dict[int | str, dict[str, list[Any]]]) -> dict[int, dict[str, list[int]]]:
    rport = {}
    for pnum, pentry in ports.items():
        rentry = {}
        for k, vlist in pentry.items():
            rlist = []
            for e in vlist:
                if isinstance(e, str):
                    rlist.extend([ord(c) for c in e])
                elif isinstance(e, int):
                    rlist.append(e)
                else:
                    raise Exception("Unsupported type '{type(e)}' in input")
            rentry[k] = rlist
        rport[int(pnum)] = rentry
    return rport


def read_input(fname: str) -> dict[int, dict[str, list[int]]]:
    with open(fname, encoding="utf-8") as file:
        ports = json.loads(file.read())
    return _ports_to_int(ports)


def write_output(fname: str, ports: dict[int, dict[str, list[int]]]) -> None:
    with open(fname, "w", encoding="utf-8") as file:
        file.write(json.dumps(_ports_to_str(ports), indent=2, sort_keys=True))
