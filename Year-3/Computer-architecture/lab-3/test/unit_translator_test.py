# pylint: disable=missing-class-docstring     # чтобы не быть Капитаном Очевидностью
# pylint: disable=missing-function-docstring  # чтобы не быть Капитаном Очевидностью
# pylint: disable=line-too-long               # строки с ожидаемым выводом

"""Модульные тесты translator
"""

import os
import tempfile
import unittest
import json

import translator


class TestIntegrational(unittest.TestCase):
    def test_lexer(self) -> None:
        input_data = """; infinite program
section .text

_start:
  ld 0, 4, _start

.loop:
  in 0 ; read from port 0
  cmp 0
  jmp .loop, ['a']

end:
  halt

section .data
"""
        expected = [
            {"ltype": "EOL", "off": 18, "raw": "\n"},
            {"ltype": "Keyword", "off": 19, "raw": "section"},
            {"ltype": "Identifier", "off": 27, "raw": ".text"},
            {"ltype": "EOL", "off": 32, "raw": "\n"},
            {"ltype": "EOL", "off": 33, "raw": "\n"},
            {"ltype": "Identifier", "off": 34, "raw": "_start"},
            {"ltype": "Syntax", "off": 40, "raw": ":"},
            {"ltype": "EOL", "off": 41, "raw": "\n"},
            {"ltype": "Identifier", "off": 44, "raw": "ld"},
            {"ltype": "NumericLiteral", "off": 47, "raw": "0"},
            {"ltype": "Syntax", "off": 48, "raw": ","},
            {"ltype": "NumericLiteral", "off": 50, "raw": "4"},
            {"ltype": "Syntax", "off": 51, "raw": ","},
            {"ltype": "Identifier", "off": 53, "raw": "_start"},
            {"ltype": "EOL", "off": 59, "raw": "\n"},
            {"ltype": "EOL", "off": 60, "raw": "\n"},
            {"ltype": "Identifier", "off": 61, "raw": ".loop"},
            {"ltype": "Syntax", "off": 66, "raw": ":"},
            {"ltype": "EOL", "off": 67, "raw": "\n"},
            {"ltype": "Identifier", "off": 70, "raw": "in"},
            {"ltype": "NumericLiteral", "off": 73, "raw": "0"},
            {"ltype": "EOL", "off": 93, "raw": "\n"},
            {"ltype": "Identifier", "off": 96, "raw": "cmp"},
            {"ltype": "NumericLiteral", "off": 100, "raw": "0"},
            {"ltype": "EOL", "off": 101, "raw": "\n"},
            {"ltype": "Identifier", "off": 104, "raw": "jmp"},
            {"ltype": "Identifier", "off": 108, "raw": ".loop"},
            {"ltype": "Syntax", "off": 113, "raw": ","},
            {"ltype": "Syntax", "off": 115, "raw": "["},
            {"ltype": "CharLiteral", "off": 116, "raw": "'a'"},
            {"ltype": "Syntax", "off": 119, "raw": "]"},
            {"ltype": "EOL", "off": 120, "raw": "\n"},
            {"ltype": "EOL", "off": 121, "raw": "\n"},
            {"ltype": "Identifier", "off": 122, "raw": "end"},
            {"ltype": "Syntax", "off": 125, "raw": ":"},
            {"ltype": "EOL", "off": 126, "raw": "\n"},
            {"ltype": "Identifier", "off": 129, "raw": "halt"},
            {"ltype": "EOL", "off": 133, "raw": "\n"},
            {"ltype": "EOL", "off": 134, "raw": "\n"},
            {"ltype": "Keyword", "off": 135, "raw": "section"},
            {"ltype": "Identifier", "off": 143, "raw": ".data"},
            {"ltype": "EOL", "off": 148, "raw": "\n"},
            {"ltype": "EOF", "off": 149, "raw": None},
        ]

        lexems = translator.lexer_process(input_data)
        self.assertEqual(len(lexems), len(expected))
        for k, lexem in enumerate(lexems):
            self.assertEqual(expected[k], lexem.__dict__)

    def test_codegen(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdirname:
            source = os.path.join(tmpdirname, "hello.asm")
            target = os.path.join(tmpdirname, "hello.asm")
            input_data = """; infinite program
section .text

_start:
  ld 0

.loop:
  in 0 ; read from port 0
  cmp 0
  jmp .loop

end:
  halt

section .data
var: 'a'
var2: 13
"""
            with open(source, "w", encoding="utf-8") as ifile:
                ifile.write(input_data)

            translator.main([source, target])

            expected_data = {
                "instructions": [
                    {"address": 0, "args": [0], "opcode": "ld_imm"},
                    {"address": 32, "args": [0], "opcode": "in_imm"},
                    {"address": 64, "args": [0], "opcode": "cmp_imm"},
                    {"address": 96, "args": [32], "opcode": "jmp"},
                    {"address": 128, "args": [], "opcode": "halt"},
                    {"address": 160, "value": 97},
                    {"address": 192, "value": 13},
                ],
                "start_pos": 0,
            }

            with open(source, "r", encoding="utf-8") as ofile:
                target_data = json.load(ofile)
                self.assertEqual(expected_data, target_data)

    # def test_code_generator(self) -> None:
