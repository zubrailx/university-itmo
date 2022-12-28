# pylint: disable=missing-class-docstring     # чтобы не быть Капитаном Очевидностью
# pylint: disable=missing-function-docstring  # чтобы не быть Капитаном Очевидностью
# pylint: disable=line-too-long               # строки с ожидаемым выводом

"""Интеграционные тесты
"""

import os
import tempfile
import unittest
import json
from typing import Any

import machine
import translator


class TestIntegrational(unittest.TestCase):
    def test_hello(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdirname:
            source = "cases/hello.asm"
            ttarget = os.path.join(tmpdirname, "hello")
            ofname = os.path.join(tmpdirname, "hello_out")
            ifname = os.path.join(tmpdirname, "hello_in")

            idata: dict[str, Any] = {"0": {"in": []}, "1": {"out": []}}

            expected_output = {
                "0": {
                    "in": [],
                },
                "1": {"out": list("hello world.\n")},
            }

            with open(ifname, "w", encoding="utf-8") as ifile:
                ifile.write(json.dumps(idata))

            translator.main([source, ttarget])
            machine.main([ttarget, ifname, ofname])

            with open(ofname, "r", encoding="utf-8") as ofile:
                self.assertEqual(json.loads(ofile.read()), expected_output)

    def test_cat(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdirname:
            source = "cases/cat.asm"
            ttarget = os.path.join(tmpdirname, "cat")
            ifname = os.path.join(tmpdirname, "cat_in-foo")
            ofname = os.path.join(tmpdirname, "cat_out-foo")

            idata: dict[str, Any] = {"0": {"in": ["foo", 0]}, "1": {"out": []}}

            expected_output = {
                "0": {
                    "in": [],
                },
                "1": {"out": list("foo")},
            }

            with open(ifname, "w", encoding="utf-8") as ifile:
                ifile.write(json.dumps(idata))

            translator.main([source, ttarget])
            machine.main([ttarget, ifname, ofname])

            with open(ofname, "r", encoding="utf-8") as ofile:
                self.assertEqual(json.loads(ofile.read()), expected_output)

    def test_prob2_in(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdirname:
            source = "cases/prob2-with-io.asm"
            ttarget = os.path.join(tmpdirname, "prob2-with-io")
            ifname = os.path.join(tmpdirname, "prob2-with-io_in")
            ofname = os.path.join(tmpdirname, "prob2-with-io_out")

            idata: dict[str, Any] = {"0": {"in": ["40\n"]}, "1": {"out": []}}

            expected_output = {"0": {"in": []}, "1": {"out": list("267914295")}}

            with open(ifname, "w", encoding="utf-8") as ifile:
                ifile.write(json.dumps(idata))

            translator.main([source, ttarget])
            machine.main([ttarget, ifname, ofname])

            with open(ofname, "r", encoding="utf-8") as ofile:
                self.assertEqual(json.loads(ofile.read()), expected_output)

    def test_cat_trace_instructions(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdirname:
            source = "cases/cat.asm"
            ttarget = os.path.join(tmpdirname, "cat")
            ifname = os.path.join(tmpdirname, "cat_in-foo")
            ofname = os.path.join(tmpdirname, "cat_out-foo")

            idata: dict[str, Any] = {"0": {"in": ["foo", 0]}, "1": {"out": []}}

            expected_output = {
                "0": {
                    "in": [],
                },
                "1": {"out": list("foo")},
            }

            expected_logs = [
                "DEBUG:root:TICK: 0, PC: 0, TC: -1, DP: { AC: -1, DR: -1, FLG: { Z: 0, N: 0 } }, IR: {'address': -1, 'opcode': None, 'args': None, 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 3, PC: 32, TC: 1, DP: { AC: -1, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 0, 'opcode': ld_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:ports[0].in >> 102 ('f')",
                "DEBUG:root:TICK: 6, PC: 64, TC: 1, DP: { AC: 0, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 9, PC: 96, TC: 1, DP: { AC: 102, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 12, PC: 128, TC: 1, DP: { AC: 102, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:ports[1].out << 102 ('f')",
                "DEBUG:root:TICK: 15, PC: 160, TC: 1, DP: { AC: 102, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 18, PC: 192, TC: 1, DP: { AC: 102, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:ports[0].in >> 111 ('o')",
                "DEBUG:root:TICK: 21, PC: 64, TC: 1, DP: { AC: 102, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 24, PC: 96, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 27, PC: 128, TC: 1, DP: { AC: 111, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:ports[1].out << 111 ('o')",
                "DEBUG:root:TICK: 30, PC: 160, TC: 1, DP: { AC: 111, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 33, PC: 192, TC: 1, DP: { AC: 111, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:ports[0].in >> 111 ('o')",
                "DEBUG:root:TICK: 36, PC: 64, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 39, PC: 96, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 42, PC: 128, TC: 1, DP: { AC: 111, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:ports[1].out << 111 ('o')",
                "DEBUG:root:TICK: 45, PC: 160, TC: 1, DP: { AC: 111, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 48, PC: 192, TC: 1, DP: { AC: 111, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:ports[0].in >> 0 ('\x00')",
                "DEBUG:root:TICK: 51, PC: 64, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 54, PC: 96, TC: 1, DP: { AC: 0, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 57, PC: 128, TC: 1, DP: { AC: 0, DR: 192, FLG: { Z: 1, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE",
                "DEBUG:root:TICK: 60, PC: 224, TC: 1, DP: { AC: 0, DR: 192, FLG: { Z: 1, N: 0 } }, IR: {'address': 192, 'opcode': halt, 'args': [], 'value': None}, STAGE: Stage.FETCH, STAT: Status.HALTED",
                "DEBUG:root:Operations count: 20",
            ]

            with open(ifname, "w", encoding="utf-8") as ifile:
                ifile.write(json.dumps(idata))

            with self.assertLogs('', level="DEBUG") as target_logs:
                translator.main([source, ttarget])
                machine.main(["-i", ttarget, ifname, ofname])

            with open(ofname, "r", encoding="utf-8") as ofile:
                self.assertEqual(json.loads(ofile.read()), expected_output)
            self.assertEqual(target_logs.output, expected_logs)
