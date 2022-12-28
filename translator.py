#!/usr/bin/python3
# pylint: disable=missing-function-docstring  # чтобы не быть Капитаном Очевидностью
# pylint: disable=invalid-name                # сохраним традиционные наименования сигналов
# pylint: disable=consider-using-f-string     # избыточный синтаксис
# pylint: disable=too-few-public-methods
# pylint: disable=too-many-public-methods

"""Транслятор Assembly в машинный код
"""


import json
import re
import sys
from ast import literal_eval
from typing import Any, Optional

from isa import ArgumentTypes, ISACommands, write_code, Instruction, \
    WORD_WIDTH, WORD_MIN_VALUE, WORD_MAX_VALUE


# Line and offset starts with 0
def get_line_off(source: str, off: int) -> tuple[int, int]:
    cur = 0
    for num, line in enumerate(source.split('\n')):
        if cur + len(line) > off:
            return num, off - cur
        cur += len(line) + 1
    return -1, -1


# ----------------------------------------------------------
# Lexer
# ----------------------------------------------------------
class LexerNode:
    """Лексемы, получаемые лексером."""
    def __init__(self, ntype: str, off: int, raw: str | None) -> None:
        self.ltype: str = ntype
        self.off: int = off
        self.raw: str | None = raw


# None - don't include identifier
LexemSpec: dict[str, tuple[Optional[str], str]] = {
    "Keyword": ("Keyword", r'section'),
    "Identifier": ("Identifier", r'[a-zA-Z_\.]\w*'),
    "Syntax": ("Syntax", r'\[|\]|:|,'),

    "NumericLiteral": ("NumericLiteral", r'[-+]?\d+'),
    "StringLiteral": ("StringLiteral", r'".*"'),
    "CharLiteral": ("CharLiteral", r"'(.|\\n|\\t|\\r|\\q)'"),

    "EOL": ("EOL", r'\n'),

    "Comment": (None, r';.*$'),
    "Whitespace": (None, r'\s+'),
}


def lexem_process(src: LexerNode) -> None:
    if src.ltype == "CharLiteral":
        assert src.raw is not None
        # replace escaped character with actual
        src.raw = src.raw[0] + literal_eval('"' + src.raw[1: -1] + '"') + src.raw[-1]


def lexer_process(source: str) -> list[LexerNode]:
    lexem_list = []
    cur_pos = 0
    while cur_pos < len(source):
        found = False
        for lexem_type, lexem_re in LexemSpec.values():
            res = re.match(lexem_re, source[cur_pos:], re.MULTILINE)
            if res is not None:
                end_pos = cur_pos + res.end()
                if lexem_type is not None:
                    node = LexerNode(lexem_type, cur_pos, source[cur_pos:end_pos])
                    lexem_process(node)
                    lexem_list.append(node)
                cur_pos = end_pos
                found = True
                break

        if not found:
            line, off = get_line_off(source, cur_pos)
            raise Exception(f"Lexer\nUnknown lexem near line:{line + 1},off:{off + 1}.")

    lexem_list.append(LexerNode('EOF', cur_pos, None))
    return lexem_list


# ----------------------------------------------------------
# Parser
# ----------------------------------------------------------
class Parser:
    """Парсер, который получая на вход список лексер формулирует дерево AST
    для дальнейшего кодогенерирования."""

    # Error handling
    @staticmethod
    def err_leaf(context: str, msg: str, lexem: LexerNode) -> dict[str, Any]:
        return {
            "type": "Error",
            "context": context,
            "message": msg,
            "lexem": lexem
        }

    @staticmethod
    def err_path(context: str, childs: list[dict[str, Any]]) -> dict[str, Any]:
        return {
            "type": "Error",
            "context": context,
            "childs": childs
        }

    @staticmethod
    def is_err(node: dict[str, Any]) -> bool:
        return bool(node["type"] == "Error")

    @staticmethod
    def val_from_label(node: dict[str, Any]) -> str:
        return str(node["value"])

    @staticmethod
    def val_from_int(node: dict[str, Any]) -> int:
        return int(node["lexem"].raw)

    @staticmethod
    def val_from_char(node: dict[str, Any]) -> int:
        return ord(node["lexem"].raw[1])

    def __init__(self) -> None:
        self.lexem_list: list[LexerNode] = []
        self.cur = 0

    def get_lex(self) -> LexerNode:
        return self.lexem_list[self.cur]

    def next_lex(self) -> LexerNode:
        self.cur += 1
        return self.lexem_list[self.cur]

    def skip_eol(self) -> None:
        while self.lexem_list[self.cur].ltype == "EOL":
            self.cur += 1

    def next_skip_eol(self) -> LexerNode:
        self.next_lex()
        self.skip_eol()
        return self.get_lex()

    # Ast builder
    # Always return pointer to there error created
    def parse(self, lexem_list: list[LexerNode]) -> dict[str, Any]:
        self.lexem_list = lexem_list
        self.cur = 0
        return self.program()

    def program(self) -> dict[str, Any]:
        sect_list = []
        last_err = None  # last error generated below
        # point to first section
        self.skip_eol()

        while self.cur != len(self.lexem_list) - 1:
            sect = self.section()
            # properly handle errors from last section
            if self.is_err(sect):
                if last_err is not None:
                    return self.err_path("Program", [last_err, sect])
                return self.err_path("Program", [sect])
            # store last error
            last_err = sect["last_err"]
            sect.pop("last_err", None)
            # append
            sect_list.append(sect)
            self.next_skip_eol()
        return {
            "type": "Program",
            "sections": sect_list,
        }

    def section(self) -> dict[str, Any]:
        start = self.cur
        root: dict[str, Any] = {
            "type": "Section",
            "value": None,  # section name
            "instructions": [],
            "last_err": None
        }
        kwd = self.keyword("section")
        if self.is_err(kwd):
            return self.err_path("Section", [kwd])

        self.next_lex()
        name = self.identifier()
        if self.is_err(name):
            self.cur = start
            return self.err_path("Section", [kwd, name])
        root["value"] = name["lexem"].raw

        self.next_skip_eol()
        inst = self.instruction()
        while not self.is_err(inst):
            root["instructions"].append(inst)
            # next should be eol
            self.next_skip_eol()
            inst = self.instruction()
        self.cur -= 1  # point to the last node of section

        root["last_err"] = inst  # inst - last error generated
        return root

    def instruction(self) -> dict[str, Any]:
        start = self.cur

        cmd = self.instr_command()
        if not self.is_err(cmd):
            return cmd

        var = self.instr_variable()
        if not self.is_err(var):
            return var

        # error
        self.cur = start
        return self.err_path("Instruction", [cmd, var])

    def instr_command(self) -> dict[str, Any]:
        start = self.cur
        root = {
            "type": "Command",
            "labels": [],
            "cmd": None,
            "args": []
        }
        # Parse labels
        label = self.instr_label()
        while not self.is_err(label):
            assert isinstance(root["labels"], list)
            root["labels"].append(self.val_from_label(label))
            self.next_skip_eol()
            label = self.instr_label()
        # Parse identifier
        cmd = self.identifier()
        if self.is_err(cmd):
            self.cur = start
            return self.err_path("Command", [cmd])

        root["cmd"] = cmd["lexem"].raw
        # Parse arguments
        self.next_lex()
        if self.is_eol():
            return root

        arg = self.argument()
        if self.is_err(arg):
            self.cur = start
            return self.err_path("Command", [arg])

        assert isinstance(root["args"], list)
        root["args"].append(arg)
        # if 1 or more
        self.next_lex()
        while not self.is_eol():
            com = self.syntax(",")
            self.next_lex()
            arg = self.argument()

            if self.is_err(com):
                self.cur = start
                return self.err_path("Command", [com])
            if self.is_err(arg):
                self.cur = start
                return self.err_path("Command", [arg])

            root["args"].append(arg)
            self.next_lex()
        return root

    def instr_variable(self) -> dict[str, Any]:
        start = self.cur
        root = {
            "type": "Variable",
            "label": None,
            "value": 0
        }
        # parse label
        label = self.instr_label()
        if self.is_err(label):
            return self.err_path("Variable", [label])

        root["label"] = self.val_from_label(label)
        # parse value
        self.next_skip_eol()
        int_lit = self.int_lit()
        if not self.is_err(int_lit):
            root["value"] = self.val_from_int(int_lit)
            return root

        char_lit = self.char_lit()
        if not self.is_err(char_lit):
            root["value"] = self.val_from_char(char_lit)
            return root
        # error
        self.cur = start
        return self.err_path("Variable", [int_lit, char_lit])

    def argument(self) -> dict[str, Any]:
        ind_arg = self.indirect_argument()
        if not self.is_err(ind_arg):
            return ind_arg
        dir_arg = self.direct_argument()
        if self.is_err(dir_arg):
            return self.err_path("Argument", [ind_arg, dir_arg])
        return dir_arg

    def direct_argument(self) -> dict[str, Any]:
        ident = self.identifier()
        if not self.is_err(ident):
            return {
                "type": ArgumentTypes.Direct,
                "value": ident["lexem"].raw
            }
        int_lit = self.int_lit()
        if not self.is_err(int_lit):
            return {
                "type": ArgumentTypes.Direct,
                "value": self.val_from_int(int_lit)
            }
        char_lit = self.char_lit()
        if not self.is_err(char_lit):
            return {
                "type": ArgumentTypes.Direct,
                "value": self.val_from_char(char_lit)
            }
        return self.err_path(ArgumentTypes.Direct, [ident, int_lit, char_lit])

    def indirect_argument(self) -> dict[str, Any]:
        start = self.cur
        brl = self.syntax("[")
        if not self.is_err(brl):
            self.next_lex()
            # identifier
            iden = self.identifier()
            if not self.is_err(iden):
                self.next_lex()
                # right bracket
                brr = self.syntax("]")
                if not self.is_err(brr):
                    return {
                        "type": ArgumentTypes.Indirect,
                        "value": iden["lexem"].raw
                    }
                err = self.err_path(ArgumentTypes.Indirect, [brl, iden, brr])
            else:
                err = self.err_path(ArgumentTypes.Indirect, [brl, iden])
        else:
            err = self.err_path(ArgumentTypes.Indirect, [brl])
        # error
        self.cur = start
        return err

    def instr_label(self) -> dict[str, Any]:
        start = self.cur

        llabel = self.identifier()
        if self.is_err(llabel):
            return self.err_path("Label", [llabel])

        self.next_lex()
        lsyn = self.syntax(":")
        if self.is_err(lsyn):
            self.cur = start
            return self.err_path("Label", [llabel, lsyn])

        return {
            "type": "Label",
            "value": llabel["lexem"].raw,
        }

    def keyword(self, value: str) -> dict[str, Any]:
        kwd = self.get_lex()
        if kwd.ltype != "Keyword":
            return self.err_leaf("Keyword", f"Got {kwd.ltype}", kwd)
        if kwd.raw != value:
            return self.err_leaf("Keyword", f"Not equals '{value}'", kwd)
        return {
            "type": "Keyword",
            "lexem": kwd
        }

    def syntax(self, value: str) -> dict[str, Any]:
        kwd = self.get_lex()
        if kwd.ltype != "Syntax":
            return self.err_leaf("Syntax", f"Got {kwd.ltype}", kwd)
        if kwd.raw != value:
            return self.err_leaf("Syntax", f"Not equals '{value}'", kwd)
        return {
            "type": "Syntax",
            "lexem": kwd
        }

    def identifier(self) -> dict[str, Any]:
        cur = self.get_lex()
        if cur.ltype != "Identifier":
            return self.err_leaf("Identifier", f"Got {cur.ltype}", cur)
        return {
            "type": "Identifier",
            "lexem": cur
        }

    def int_lit(self) -> dict[str, Any]:
        cur = self.get_lex()
        if cur.ltype != "NumericLiteral":
            return self.err_leaf("IntLiteral", f"Got {cur.ltype}", cur)
        return {
            "type": "NumericLiteral",
            "lexem": cur
        }

    def char_lit(self) -> dict[str, Any]:
        cur = self.get_lex()
        if cur.ltype != "CharLiteral":
            return self.err_leaf("CharLiteral", f"Got {cur.ltype}", cur)
        return {
            "type": "CharLiteral",
            "lexem": cur
        }

    def is_eol(self) -> bool:
        cur = self.get_lex()
        return cur.ltype == "EOL"


def parse(lexem_list: list[LexerNode]) -> dict[str, Any]:
    parser = Parser()
    ast = parser.parse(lexem_list)

    if ast["type"] == "Error":
        raise Exception(f"Parser\n{json.dumps(ast, indent=2, default=lambda o: o.__dict__)}")
    return ast


# ----------------------------------------------------------
# Code generator
# ----------------------------------------------------------
# Valid instructions in sections
SectionInstrTypes = {
    ".data": ["Variable"],
    ".text": ["Command"],
}


# Check semantics and generate code
def _int_out_of_bounds(val: int) -> bool:
    return bool(val > WORD_MAX_VALUE or val < WORD_MIN_VALUE)


def check_bounds(inst: dict[str, Any]) -> None:
    if inst["type"] == "Command":
        args = [arg["value"] for arg in inst["args"]]
        for arg in args:
            if isinstance(arg, int):
                if _int_out_of_bounds(arg):
                    raise Exception(f"Argument '{arg}' of command '{inst['cmd']}' is out of bounds")
    elif inst["type"] == "Variable":
        val = inst["value"]
        if _int_out_of_bounds(val):
            raise Exception(f"Argument '{val}' of command '{inst['cmd']}' is out of bounds")


def check_section_instructions(ast: dict[str, Any]) -> None:
    for sect in ast["sections"]:
        sect_name = sect["value"]
        if sect_name not in SectionInstrTypes:
            raise Exception(f"Section '{sect_name}' not in permitted")

        for inst in sect["instructions"]:
            if inst["type"] not in SectionInstrTypes[sect_name]:
                raise Exception(f"Instruction type of {inst} in '{sect_name}' is not permitted")
            # check variable bounds
            check_bounds(inst)


def inst_to_isa_pre(inst: dict[str, Any]) -> Instruction:
    if inst["type"] == "Command":
        name = inst["cmd"]
        isa_args = [arg["type"] for arg in inst["args"]]
        # command
        cmd = ISACommands.get_command(name)
        if cmd is None:
            raise Exception(f"Command '{name}' is not present in isa")
        # opcode
        opcode = cmd.get_opcode(tuple(isa_args))
        if opcode is None:
            raise Exception(f"Command '{name}' doesn't support {isa_args}. Given args:"
                            "{inst['args']}\n" f"List of supported: {cmd.get_command_vars()}")
        return Instruction(
            address=-1,
            opcode=opcode,
            args=[arg["value"] for arg in inst["args"]]
        )
    # else variable
    return Instruction(
        address=-1,
        value=inst["value"]
    )


def generate_code(ast: dict[str, Any], start_pos: int = 0) -> dict[str, Any]:
    check_section_instructions(ast)
    inst_list: list[Instruction] = []
    labels = {}
    # copy all data but without sections
    for sect in ast["sections"]:
        for inst in sect["instructions"]:
            # convert to isa
            isa = inst_to_isa_pre(inst)
            isa.address = start_pos
            inst_list.append(isa)
            # add labels
            if inst["type"] == "Variable":
                inst_labels = [inst["label"]]
            else:
                inst_labels = inst["labels"]

            for label in inst_labels:
                if label in labels:
                    raise Exception(f"Label '{label}' is duplicated")
                labels[label] = isa.address
            # goto next insruction
            start_pos += WORD_WIDTH

    # replace instruction addresses
    for inst in inst_list:
        if inst.args is not None:
            for idx, arg in enumerate(inst.args):
                if arg in labels:
                    inst.args[idx] = labels[arg]
                elif not isinstance(arg, int):
                    raise Exception(f"Label '{arg}' not found")

    return {
        "instructions": inst_list,
        "start_pos": labels.get("_start", 0)
    }


# ----------------------------------------------------------
# Main
# ----------------------------------------------------------
def main(args: list[str]) -> None:
    assert len(args) == 2, \
        "Wrong arguments: translator.py <input_file> <target_file>"
    source, target = args

    with open(source, "rt", encoding="utf-8") as f:
        source = f.read()

    lexem_list = lexer_process(source)
    ast = parse(lexem_list)
    code = generate_code(ast)
    write_code(target, code["instructions"], code["start_pos"])


if __name__ == '__main__':
    main(sys.argv[1:])
