# pylint: disable=missing-class-docstring     # чтобы не быть Капитаном Очевидностью
# pylint: disable=missing-function-docstring  # чтобы не быть Капитаном Очевидностью
# pylint: disable=line-too-long               # строки с ожидаемым выводом

"""Модульные тесты ISA
"""

import os
import tempfile
import unittest
import json

import isa


class TestIsa(unittest.TestCase):
    def test_code(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdirname:
            fname = os.path.join(tmpdirname, "isa_input")

            start_pos = 32
            instr_list = [isa.Instruction(0, isa.Opcode.LD_IMM, [0], value=None)]

            isa.write_code(fname, instr_list, start_pos)
            instr_list_target, start_pos_target = isa.read_code(fname)

            self.assertEqual(start_pos, start_pos_target)
            self.assertEqual(len(instr_list_target), len(instr_list))
            for idx, instr in enumerate(instr_list):
                self.assertEqual(instr_list_target[idx].value, instr.value)
                self.assertEqual(instr_list_target[idx].address, instr.address)
                self.assertEqual(instr_list_target[idx].opcode, instr.opcode)
                self.assertEqual(instr_list_target[idx].args, instr.args)

    def test_input(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdirname:
            fname = os.path.join(tmpdirname, "isa_input")

            input_data = {0: {"in": ["10", 0]}, "1": {"out": [12, 0]}}

            with open(fname, "w", encoding="utf-8") as ifile:
                ifile.write(json.dumps(input_data))

            target_data = isa.read_input(fname)

            expected_data = {
                0: {"in": [ord(c) for c in "10"] + [0]},
                1: {"out": [12, 0]},
            }
            self.assertEqual(target_data, expected_data)
