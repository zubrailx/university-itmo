# pylint: disable=missing-class-docstring     # чтобы не быть Капитаном Очевидностью
# pylint: disable=missing-function-docstring  # чтобы не быть Капитаном Очевидностью
# pylint: disable=line-too-long               # строки с ожидаемым выводом
# pylint: disable=protected-access

"""Модульные тесты machine
"""

import unittest

from isa import WORD_MIN_VALUE, WORD_MAX_VALUE
from machine import Alu

Flags = Alu.Flags
Op = Alu.Operations


class TestMachine(unittest.TestCase):
    def test_flags(self) -> None:
        alu = Alu()
        alu._set_bit(1, Flags.ZERO)
        self.assertEqual(alu.get_bit(Flags.ZERO), 1)
        alu._set_bit(0, Flags.ZERO)
        self.assertEqual(alu.get_bit(Flags.ZERO), 0)
        alu._set_bit(1, Flags.NEG)
        self.assertEqual(alu.get_bit(Flags.ZERO), 0)
        self.assertEqual(alu.get_bit(Flags.NEG), 1)
        alu._set_bit(0, Flags.NEG)
        self.assertEqual(alu.get_bit(Flags.ZERO), 0)
        self.assertEqual(alu.get_bit(Flags.NEG), 0)

    def test_alu(self) -> None:
        alu = Alu()
        alu.perform(Op.ADD, 10, 10)
        self.assertEqual(alu.get_bit(Flags.ZERO), 0)
        self.assertEqual(alu.get_bit(Flags.NEG), 0)
        alu.perform(Op.SUB, 10, 10)
        self.assertEqual(alu.get_bit(Flags.ZERO), 1)
        self.assertEqual(alu.get_bit(Flags.NEG), 0)
        alu.perform(Op.ADD, WORD_MAX_VALUE, 1)
        self.assertEqual(alu.get_bit(Flags.NEG), 1)
        self.assertEqual(alu.get_bit(Flags.ZERO), 0)
        alu.perform(Op.SUB, WORD_MIN_VALUE, 1)
        self.assertEqual(alu.get_bit(Flags.NEG), 0)
        self.assertEqual(alu.get_bit(Flags.ZERO), 0)
