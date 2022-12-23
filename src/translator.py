#!/usr/bin/python3

import sys
from collections import namedtuple
from enum import Enum
import re
import json

from isa import write_code


# Line and offset starts with 0
def get_line_off(source, off):
    cur = 0
    for num, line in enumerate(source.split('\n')):
        if cur + len(line) > off:
            return num, off - cur
        cur += len(line) + 1
    return -1, -1


# ----------------------------------------------------------
# Lexer
# ----------------------------------------------------------
class LexerNode(namedtuple('LexerNode', 'type off raw')):
    pass


# None - don't include identifier
LexemSpec = {
    "Keyword": ["Keyword", r'section'],
    "Identifier": ["Identifier", r'[a-zA-Z_\.][\w]*'],
    "Syntax": ["Syntax", r'\[|\]|:|,'],

    "NumericLiteral": ["NumericLiteral", r'[-+]?\d+'],
    "StringLiteral": ["StringLiteral", r'".*"'],
    "CharLiteral": ["CharLiteral", r"'(.|\\n|\\t)'"],

    "EOL": ["EOL", r'\n'],

    "Comment": [None, r';.*$'],
    "Whitespace": [None, r'\s+'],
}


def lexer_process(source):
    lexem_list = []
    cur_pos = 0
    while cur_pos < len(source):
        found = False
        for lexem_type, lexem_re in LexemSpec.values():
            res = re.match(lexem_re, source[cur_pos:], re.MULTILINE)
            if res is not None:
                end_pos = cur_pos + res.end()
                if lexem_type is not None:
                    lexem_list.append(LexerNode(lexem_type, cur_pos, source[cur_pos:end_pos]))

                cur_pos = end_pos
                found = True
                break

        if not found:
            line, off = get_line_off(source, cur_pos)
            print(f"ERROR[lexer]: Unknown lexem near line:{line + 1},off:{off + 1}.")
            return None

    lexem_list.append(LexerNode('EOF', cur_pos, None))
    return lexem_list


# ----------------------------------------------------------
# Parser
# ----------------------------------------------------------
class Parser:
    # Error handling
    @staticmethod
    def err_leaf(context: str, msg: str, lexem: LexerNode) -> dict:
        return {
            "type": "Error",
            "context": context,
            "message": msg,
            "lexem": lexem
        }

    @staticmethod
    def err_path(context: str, childs: list) -> dict:
        return {
            "type": "Error",
            "context": context,
            "childs": childs
        }

    @staticmethod
    def is_err(node: dict) -> bool:
        return node["type"] == "Error"

    @staticmethod
    def val_from_label(node: dict) -> str:
        return node["value"]

    @staticmethod
    def val_from_int(node: dict) -> int:
        return int(node["lexem"].raw)

    @staticmethod
    def val_from_char(node: dict) -> int:
        return ord(node["lexem"].raw[1])

    def __init__(self) -> None:
        self.lexem_list = []
        self.cur = 0

    def get_lex(self) -> LexerNode:
        return self.lexem_list[self.cur]

    def next_lex(self) -> LexerNode:
        self.cur += 1
        return self.lexem_list[self.cur]

    def skip_eol(self) -> None:
        while self.lexem_list[self.cur].type == "EOL":
            self.cur += 1

    def next_skip_eol(self) -> LexerNode:
        self.next_lex()
        self.skip_eol()
        return self.get_lex()

    # Ast builder
    # Always return pointer to there error created
    def parse(self, lexem_list: list):
        self.lexem_list = lexem_list
        self.cur = 0
        return self.program()

    def program(self):
        sect_list = []
        last_err = None # last error generated below
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
            "body": sect_list,
        }

    def section(self):
        start = self.cur
        root = {
            "type": "Section",
            "value": None,  # section name
            "inst": [],
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
        else:
            root["value"] = name["lexem"].raw

        self.next_skip_eol()
        inst = self.instruction()
        while not self.is_err(inst):
            root["inst"].append(inst)
            # next should be eol
            self.next_skip_eol()
            inst = self.instruction()
        self.cur -= 1  # point to the last node of section

        root["last_err"] = inst # inst - last error generated
        return root

    def instruction(self):
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

    def instr_command(self):
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
        else:
            arg = self.argument()
            if self.is_err(arg):
                self.cur = start
                return self.err_leaf("Command", "Expected argument", arg["lexem"])
            else:
                root["args"].append(arg)
        # if 1 or more
        self.next_lex()
        while not self.is_eol():
            com = self.syntax(",")
            self.next_lex()
            arg = self.argument()

            if self.is_err(com):
                self.cur = start
                return self.err_leaf("Command", "Expected ','", com["lexem"])
            elif self.is_err(arg):
                self.cur = start
                return self.err_leaf("Command", "Expected argument", arg["lexem"])

            root["args"].append(arg)
            self.next_lex()
        return root

    def instr_variable(self):
        start = self.cur
        root = {
            "type": "Variable",
            "label": None,
            "value": None
        }
        # parse label
        label = self.instr_label()
        if self.is_err(label):
            return self.err_path("Variable", [label])
        else:
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

    def argument(self):
        ind_arg = self.indirect_argument()
        if not self.is_err(ind_arg):
            return ind_arg
        dir_arg = self.direct_argument()
        if not self.is_err(dir_arg):
            return dir_arg
        else:
            return self.err_path("Argument", [ind_arg, dir_arg])

    def direct_argument(self):
        ident = self.identifier()
        if not self.is_err(ident):
            return {
                "type": "DirectArgument",
                "value": ident["lexem"].raw
            }
        int_lit = self.int_lit()
        if not self.is_err(int_lit):
            return {
                "type": "DirectArgument",
                "value": self.val_from_int(int_lit)
            }
        char_lit = self.char_lit()
        if not self.is_err(char_lit):
            return {
                "type": "DirectArgument",
                "value": self.val_from_char(char_lit)
            }
        return self.err_path("DirectArgument", [ident, int_lit, char_lit])

    def indirect_argument(self):
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
                        "type": "IndirectArgument",
                        "value": iden["lexem"].raw
                    }
                else:
                    err = self.err_path("IndirectArgument", [brl, iden, brr])
            else:
                err = self.err_path("IndirectArgument", [brl, iden])
        else:
            err = self.err_path("IndirectArgument", [brl])
        # error
        self.cur = start
        return err

    def instr_label(self):
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

    def keyword(self, value):
        kwd = self.get_lex()
        if kwd.type != "Keyword":
            return self.err_leaf("Keyword", f"Got {kwd.type}", kwd)
        elif kwd.raw != value:
            return self.err_leaf("Keyword", f"Not equals '{value}'", kwd)
        else:
            return {
                "type": "Keyword",
                "lexem": kwd
            }

    def syntax(self, value):
        kwd = self.get_lex()
        if kwd.type != "Syntax":
            return self.err_leaf("Syntax", f"Got {kwd.type}", kwd)
        elif kwd.raw != value:
            return self.err_leaf("Syntax", f"Not equals '{value}'", kwd)
        else:
            return {
                "type": "Syntax",
                "lexem": kwd
            }

    def identifier(self):
        cur = self.get_lex()
        if cur.type != "Identifier":
            return self.err_leaf("Identifier", f"Got {cur.type}", cur)
        else:
            return {
                "type": "Identifier",
                "lexem": cur
            }

    def int_lit(self):
        cur = self.get_lex()
        if cur.type != "NumericLiteral":
            return self.err_leaf("IntLiteral", f"Got {cur.type}", cur)
        else:
            return {
                "type": "NumericLiteral",
                "lexem": cur
            }

    def char_lit(self):
        cur = self.get_lex()
        if cur.type != "CharLiteral":
            return self.err_leaf("CharLiteral", f"Got {cur.type}", cur)
        else:
            return {
                "type": "CharLiteral",
                "lexem": cur
            }

    def is_eol(self):
        cur = self.get_lex()
        return cur.type == "EOL"



def parse(lexem_list):
    parser = Parser()
    ast = parser.parse(lexem_list)
    if ast["type"] == "Error":
        print("\nERROR WHILE PARSING:")
    print(json.dumps(ast, indent=2))


def generate_code(ast):
    return "CODE GENERATION IS NOT IMPLEMENTED"


def main(args):
    args = []
    args.extend(['../test/prob2.asm', '../test/hello'])

    assert len(args) == 2, \
        "Wrong arguments: translator.py <input_file> <target_file>"
    source, target = args

    with open(source, "rt", encoding="utf-8") as f:
        source = f.read()

    lexem_list = lexer_process(source)
    print(lexem_list)
    ast = parse(lexem_list)
    code = generate_code(ast)
    # write_code(target, code)


if __name__ == '__main__':
    main(sys.argv[1:])
