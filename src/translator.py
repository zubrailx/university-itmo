#!/usr/bin/python3

import sys
from collections import namedtuple
from enum import Enum
import re

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
    "Syntax": ["Syntax", r'\[|\]|:'],

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
# lexem - is present only for terminals
# childs - list or children
class ParserNode(namedtuple('ParserNode', 'lexem type childs')):
    pass


# program ::= {section}
# section ::= keyword(section) identifier { instruction }
# label ::= identifier, ":"
# terminal
# instruction ::= {label} identifier {operand}

# indirect_op ::= keyword([) identifier keyword(])
# direct_op ::= identifier | int_lit | char_lit

# identifier

# int_lit ::= terminal
# char_lit :: = terminal
# keyword ::= terminal
class ParserTypes(Enum):
    Program = 0,

    Section = 1,

    Label = 2,
    Identifier = 3
    Instruction = 3,

    IndirectOperand = 41,
    DirectOperand = 42,

    NumericLiteral = 5,
    CharLiteral = 6,
    StringLiteral = 7,

    Keyword = 8,


class Parser:
    # Error handling
    @staticmethod
    def err_leaf(msg, lexem):
        return {
            "type": "Error",
            "message": msg,
            "lexem": lexem
        }

    @staticmethod
    def err_path(childs):
        return {
            "type": "Error",
            "childs": childs
        }

    @staticmethod
    def is_err(node):
        return node["type"] == "Error"

    @staticmethod
    def val_from_int(node):
        return int(node["lexem"].raw)

    @staticmethod
    def val_from_char(node):
        return ord(node["lexem"].raw[1])

    def __init__(self) -> None:
        self.lexem_list = []
        self.cur = 0

    def get_lex(self):
        return self.lexem_list[self.cur]

    def next_lex(self):
        self.cur += 1
        return self.lexem_list[self.cur]

    def prev_lex(self):
        self.cur -= 1
        return self.lexem_list[self.cur]

    def skip_eol(self):
        while self.lexem_list[self.cur].type == "EOL":
            self.cur += 1

    def next_skip_eol(self):
        self.next_lex()
        self.skip_eol()
        return self.get_lex()

    # Ast builder
    # Always return pointer to there error created
    def parse(self, lexem_list):
        self.lexem_list = lexem_list
        self.cur = 0
        return self.program()

    def program(self):
        start = self.cur
        sect_list = []

        self.skip_eol()
        lcur = self.get_lex()
        while lcur["type"] != "EOF":
            sect = self.section()
            if self.is_err(sect):
                self.cur = start  # return pointer where no errors
                return self.err_path(sect)
            else:
                lcur = self.next_skip_eol()
                sect_list.append(sect)
        return {
            "type": "Program",
            "body": sect_list,
        }

    def section(self):
        start = self.cur
        root = {
            "type": "Section",
            "value": None,  # section name
            "inst": []
        }
        kwd = self.keyword("section")
        if self.is_err(kwd):
            return self.err_path([kwd])

        self.next_lex()
        name = self.identifier()
        if self.is_err(name):
            self.cur = start
            return self.err_path([name])
        else:
            root["value"] = name["lexem"].raw

        self.next_skip_eol()
        inst = self.instruction()
        while not self.is_err(inst):
            root["inst"].append(inst)
            self.next_skip_eol()
            inst = self.instruction()
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
        return self.err_path([cmd, var])

    def instr_command(self):
        start = self.cur
        root = {
            "type": "Command",
            "labels": [],
            "cmd": None,
            "args": []
        }
        # parse labels
        label = self.instr_label()
        while not self.is_err(label):
            root["labels"].append(label)
            self.next_skip_eol()
            label = self.instr_label()
        # parse identifier
        cmd = self.identifier()
        if self.is_err(cmd):
            self.cur = start
            return self.err_path([cmd])

        root["cmd"] = cmd["lexem"].raw
        # parse arguments
        self.next_skip_eol()
        arg = self.argument()
        while not self.is_err(arg):
            root["args"].append(arg)
            self.next_lex()
            arg = self.argument()
        return root

    def instr_variable(self):
        start = self.cur
        root = {
            "type": "Variable",
            "label": None,
            "value": None
        }
        label = self.instr_label()
        if self.is_err(label):
            return self.err_path([label])

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
        return self.err_path([int_lit, char_lit])

    def argument(self):
        ind_arg = self.indirect_argument()
        if not self.is_err(ind_arg):
            return ind_arg
        dir_arg = self.direct_argument()
        if not self.is_err(dir_arg):
            return dir_arg
        else:
            return self.err_path([ind_arg, dir_arg])

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
        return self.err_path([ident, int_lit, char_lit])

    def indirect_argument(self):
        start = self.cur
        brl = self.keyword("[")
        if not self.is_err(brl):
            self.next_lex()
            # identifier
            iden = self.identifier()
            if not self.is_err(iden):
                self.next_lex()
                # right bracket
                brr = self.keyword("]")
                if not self.is_err(brr):
                    return {
                        "type": "IndirectArgument",
                        "value": iden["lexem"].raw
                    }
                else:
                    err = self.err_path([brl, iden, brr])
            else:
                err = self.err_path([brl, iden])
        else:
            err = self.err_path([brl])
        # error
        self.cur = start
        return err

    def instr_label(self):
        start = self.cur

        llabel = self.identifier()
        if self.is_err(llabel):
            return self.err_path([llabel])

        llabel = self.get_lex()
        if llabel.type != "Identifier":
            return self.err_leaf("Lexem doesn't match Identifier", llabel)

        lsyn = self.next_lex()
        if lsyn.type != "Syntax" or lsyn.raw != ":":
            self.cur = start
            return self.err_leaf("Lexem doesn't match ':'", lsyn)

        return {
            "type": "Label",
            "value": llabel,
        }

    def keyword(self, value):
        kwd = self.get_lex()
        if kwd.type != "Keyword":
            return self.err_leaf("Lexem doesn't match Keyword", kwd)
        elif kwd.raw != value:
            return self.err_leaf(f"Lexem doesn't equals '{value}'", kwd)
        else:
            return {
                "type": "Keyword",
                "lexem": kwd
            }

    def identifier(self):
        lid = self.get_lex()
        if lid.type != "Identifier":
            return self.err_leaf("Lexem doesn't match Identifier", lid)
        else:
            return {
                "type": "Identifier",
                "lexem": lid
            }

    def int_lit(self):
        cur = self.get_lex()
        if cur.type != "IntLiteral":
            return self.err_leaf("Lexem doesn't match IntLiteral", cur)
        else:
            return {
                "type": "IntLiteral",
                "lexem": cur
            }

    def char_lit(self):
        cur = self.get_lex()
        if cur.type != "CharLiteral":
            return self.err_leaf("Lexem doesn't match CharLiteral", cur)
        else:
            return {
                "type": "CharLiteral",
                "lexem": cur
            }


def parse(lexem_list):
    parser = Parser()
    return parser.parse(lexem_list)


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
    ast = parse(lexem_list)
    code = generate_code(ast)
    write_code(target, code)


if __name__ == '__main__':
    main(sys.argv[1:])
