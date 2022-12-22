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

#----------------------------------------------------------
# Lexer
#----------------------------------------------------------
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
    while (cur_pos < len(source)):
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

#----------------------------------------------------------
# Parser
#----------------------------------------------------------
# lexem - is present only for terminals
# childs - list or children
class ParserNode(namedtuple('ParserNode', 'lexem type childs')):
    pass

class ParserTypes(Enum):
    Program = 0,
    
    


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
    print(get_line_off(source, 74))
    ast = parser_process(lexem_list)
    code = generate_code(ast)
    write_code(target, code)

if __name__ == '__main__':
    main(sys.argv[1:])
