#!/usr/bin/python3

from isa import Opcode, read_code, Instruction, WORD_WIDTH, read_input, write_output

import logging
import sys
from enum import Enum

from typing import TypeVar, Generic

T = TypeVar('T')

WORD_MAX_VALUE = 2 ** (WORD_WIDTH - 1) - 1
WORD_MIN_VALUE = 2 ** (WORD_WIDTH - 1) * (-1)
TICK_LIMIT = 5000
ITOC_CONST = ord('0')


# Utility
# Wrapper to pass to methods values by reference
class Wrapper(Generic[T]):
    def __init__(self, val: T) -> None:
        self.val: T = val

    def get(self) -> T:
        return self.val

    def set(self, val: T) -> None:
        self.val = val

    def __eq__(self, o) -> bool:
        if type(self) == type(o):
            return self.val == o.val
        elif type(self.val) == type(o):
            return self.val == o
        else:
            assert 0, f"Equal operator doesn't support operands '{type(self)}' '{type(o)}'"
            return False

    def __repr__(self) -> str:
        return self.val.__repr__()


# Trigger to pass value on if state_prev == High and state_cur == Low
class Latch(Generic[T]):
    # target - wrapper, because can't pass value by pointer
    def __init__(self, target: Wrapper[T]):
        self.latched: T | None = None
        self.was_high: bool = False
        self.target: Wrapper[T] = target

    # if False then pass value to latch else 
    def latch(self, is_high: bool, value: T | None = None):
        if self.was_high and not is_high:
            assert self.latched is not None
            self.target.set(self.latched)
        else:
            self.latched = value
        # store last state
        self.was_high = is_high


class Alu:
    class Operations(Enum):
        ADD = 0,
        SUB = 1,
        MUL = 2,
        DIV = 3,
        MOD = 4,
        RIGHT = 5,
        LEFT = 6

    class Flags(int, Enum):
        # idx of bit
        ZERO = 0,
        NEG = 1

    def __init__(self) -> None:
        self.flags = 0b0
        self.operations: dict = {
            self.Operations.ADD: lambda l, r: l + r,
            self.Operations.SUB: lambda l, r: l - r,
            self.Operations.MUL: lambda l, r: l * r,
            self.Operations.DIV: lambda l, r: l // r,
            self.Operations.MOD: lambda l, r: l % r,
            self.Operations.RIGHT: lambda _, r: r,
            self.Operations.LEFT: lambda l, _: l
        }

    def get_bit(self, flag):
        return self.flags >> flag.value

    def _set_bit(self, x: int, flag):
        n: int = flag.value
        self.flags = self.flags & ~(1 << n) | (x << n)

    def _post_set_flags(self, res: int):
        self._set_bit(res == 0, Alu.Flags.ZERO)
        self._set_bit(res < 0, Alu.Flags.NEG)

    @staticmethod
    def _fix_overflow(res: int):
        if res > WORD_MAX_VALUE:
            return res + WORD_MIN_VALUE + WORD_MAX_VALUE
        elif res < WORD_MIN_VALUE:
            return res + WORD_MAX_VALUE - WORD_MIN_VALUE
        else:
            return res

    def perform(self, op: Operations, left: int, right: int) -> int:
        assert op in self.operations, \
            f"Operation {op} is not present in alu operations."

        res = self.operations[op](left, right)
        res = self._fix_overflow(res)
        self._post_set_flags(res)
        return res


class DataPath:
    # ports - buses connected to ports where on device side:
    #   ports[num][0] - in, ports[num][1] - out
    def __init__(self, memory: list, ports: dict[str, dict[str, list[int]]]) -> None:
        assert len(memory), "Should be at least one instruction"
        self.memory: list[Instruction] = memory

        self.ports_in: dict[str, list[int]] = {}
        self.ports_out: dict[str, list[int]] = {}
        for num, port in ports.items():
            if "in" in port:
                self.ports_in[num] = port["in"]
            if "out" in port:
                self.ports_out[num] = port["out"]

        self.alu: Alu = Alu()
        # registers
        self.ac: Wrapper[int] = Wrapper[int](-1)  # accumulator
        self.dr: Wrapper[int] = Wrapper[int](-1)  # data register

    def _get_mem_cell(self, addr: int):
        lb = 0
        rb = len(self.memory) - 1
        while lb <= rb:
            m = (lb + rb) // 2
            cell = self.memory[m]
            if cell.address < addr:
                lb = m + 1
            elif cell.address > addr:
                rb = m - 1
            else:
                return cell

        raise Exception(f"DataPath: No instructions found with address '{addr}'")

    def memory_perform(self, addr: int, data_in: int = 0, oe: bool = False, wr: bool = False) -> Instruction:
        cell = self._get_mem_cell(addr)
        if wr:
            if cell.value is not None:
                cell.value = data_in
            else:
                raise Exception(f"DataPath: Trying to overwrite code on address '{addr}")
        if oe:
            return cell

        assert oe or wr, "Invalid arguments to memory_perform. Should be oe=True or wr=True"
        # never will get here if everything is ok
        return Instruction()

    def alu_perform(self, op: Alu.Operations, left: int, right: int) -> int:
        return self.alu.perform(op, left, right)

    def port_perform(self, port: int, io_wr: bool):
        # convert port from int to str
        port = str(port)
        if io_wr:
            pout = self.ac.get()
            logging.debug(f"ports[{port}].out << {pout}")
            self.ports_out[port].append(pout)
        else:
            if len(self.ports_in[port]) == 0:
                raise Exception(f"DataPath: ports[{port}].in queue is empty")
            pin = self.ports_in[port].pop(0)
            logging.debug(f"ports[{port}].in >> {pin}")
            self.ac.set(pin)

    def __repr__(self) -> str:
        return "AC: {}, DR: {}".format(
            self.ac,
            self.dr
        )


class ControlUnit:
    class Status(Enum):
        RUNNABLE = 0,
        HALTED = 1,
        TERMINATED = 2

    class Stage(Enum):
        FETCH = 0,
        DECODE = 1,
        EXECUTE = 2

    def __init__(self, data_path: DataPath, start_pos: int) -> None:
        self.data_path: DataPath = data_path
        # registers
        self.program_counter: Wrapper[int] = Wrapper[int](start_pos)
        self.tick_counter: Wrapper[int] = Wrapper[int](-1)  # for instruction
        self.instr_reg: Instruction = Instruction()  # struct format

        self.stage: ControlUnit.Stage = ControlUnit.Stage.FETCH

        self._status = ControlUnit.Status.RUNNABLE
        self._tick: int = 0
        # to emulate latch
        self.pc_latch: Latch[int] = Latch[int](self.program_counter)
        self.tc_latch: Latch[int] = Latch[int](self.tick_counter)
        self.ac_latch: Latch[int] = Latch[int](self.data_path.ac)
        self.dr_latch: Latch[int] = Latch[int](self.data_path.dr)

    def get_tick(self):
        return self._tick

    def get_status(self):
        return self._status

    # Instruction Cycle
    def fetch(self):
        cmd = self.data_path.memory_perform(self.program_counter.get(), oe=True)
        self.instr_reg = cmd
        self.tick_counter.set(0)
        self.pc_latch.latch(True, self.program_counter.get() + WORD_WIDTH)

    # cycle ended or not
    def decode(self) -> bool:
        self.pc_latch.latch(False)
        self.tc_latch.latch(False)

        cmd: Instruction = self.instr_reg
        op = cmd.opcode

        if self.tick_counter.get() == 0:
            self.tc_latch.latch(True, self.tick_counter.get() + 1)
            # commands with at least one argument
            if op not in [Opcode.INC, Opcode.DEC, Opcode.CTOI, Opcode.ITOC, Opcode.HALT]:
                self.data_path.dr.set(cmd.args[0])
            # indirect commands
            if op in [Opcode.ADD_M, Opcode.SUB_M, Opcode.MOD_M, Opcode.MUL_M,
                      Opcode.LD_M, Opcode.CMP_M]:
                return False
            else:
                return True
        elif self.tick_counter.get() == 1:
            self.tc_latch.latch(True, self.tick_counter.get() + 1)
            # load indirect argument
            data = self.data_path.memory_perform(self.data_path.dr.get(), oe=True).value
            self.dr_latch.latch(True, data)
            return True
        # else
        assert 0, "Decoder should last max 2 ticks"
        return True

    # cycle ended or not
    def execute(self) -> bool:
        self.tc_latch.latch(False)
        self.dr_latch.latch(False)
        cmd: Instruction = self.instr_reg
        op = cmd.opcode

        if op == Opcode.HALT:
            self._status = ControlUnit.Status.HALTED
            return True

        # Accumulator modification commands
        if op in [Opcode.INC, Opcode.DEC, Opcode.ITOC, Opcode.CTOI]:
            ac_old: int = self.data_path.ac.get()
            ac_new: int = -1
            match op:
                case Opcode.INC:
                    ac_new = self.data_path.alu_perform(Alu.Operations.ADD, ac_old, 1)
                case Opcode.DEC:
                    ac_new = self.data_path.alu_perform(Alu.Operations.SUB, ac_old, 1)
                case Opcode.ITOC:
                    ac_new = self.data_path.alu_perform(Alu.Operations.ADD, ac_old, ITOC_CONST)
                case Opcode.CTOI:
                    ac_new = self.data_path.alu_perform(Alu.Operations.SUB, ac_old, ITOC_CONST)
            self.ac_latch.latch(True, ac_new)
            return True

        # Accumulator + data_reg modification commands
        if op in [Opcode.ADD_M, Opcode.ADD_IMM, Opcode.SUB_M, Opcode.SUB_IMM, Opcode.DIV_M,
                  Opcode.DIV_IMM, Opcode.MOD_M, Opcode.MOD_IMM, Opcode.MUL_M, Opcode.MUL_IMM,
                  Opcode.CMP_IMM, Opcode.CMP_M]:
            ac_old: int = self.data_path.ac.get()
            ac_new: int = -1
            dr_val = self.data_path.dr.get()

            if op in [Opcode.CMP_M, Opcode.CMP_IMM]:
                self.data_path.alu_perform(Alu.Operations.SUB, ac_old, dr_val)
                return True

            match op:
                case Opcode.ADD_M | Opcode.ADD_IMM:
                    ac_new = self.data_path.alu_perform(Alu.Operations.ADD, ac_old, dr_val)
                case Opcode.SUB_M | Opcode.SUB_IMM:
                    ac_new = self.data_path.alu_perform(Alu.Operations.SUB, ac_old, dr_val)
                case Opcode.MUL_M | Opcode.MUL_IMM:
                    ac_new = self.data_path.alu_perform(Alu.Operations.MUL, ac_old, dr_val)
                case Opcode.DIV_M | Opcode.DIV_IMM:
                    ac_new = self.data_path.alu_perform(Alu.Operations.DIV, ac_old, dr_val)
                case Opcode.MOD_M | Opcode.MOD_IMM:
                    ac_new = self.data_path.alu_perform(Alu.Operations.MOD, ac_old, dr_val)
            self.ac_latch.latch(True, ac_new)
            return True

        # Load value in accumulator from mem
        if op in [Opcode.LD_M, Opcode.LD_IMM]:
            dr_val = self.data_path.dr.get()
            self.data_path.ac.set(self.data_path.alu_perform(Alu.Operations.RIGHT, 0, dr_val))
            return True

        # Store value from accumulator in mem
        if op in [Opcode.ST_IMM]:
            dr_val = self.data_path.dr.get()
            ac_val = self.data_path.ac.get()
            self.data_path.memory_perform(dr_val, ac_val, wr=True)
            return True

        # Jumps
        if op in [Opcode.JE, Opcode.JNE, Opcode.JS, Opcode.JMP]:
            do_jump: bool = True
            match op:
                case Opcode.JE:
                    do_jump = self.data_path.alu.get_bit(Alu.Flags.ZERO) == 1
                case Opcode.JNE:
                    do_jump = self.data_path.alu.get_bit(Alu.Flags.ZERO) == 0
                case Opcode.JS:
                    do_jump = self.data_path.alu.get_bit(Alu.Flags.NEG) == 1
            if do_jump:
                self.program_counter.set(self.data_path.dr.get())
            return True

        # Port mapped IO
        if op in [Opcode.IN_IMM, Opcode.OUT_IMM]:
            port = self.data_path.dr.get()
            self.data_path.port_perform(port, op == Opcode.OUT_IMM)
            return True

        raise Exception(f"Reached unknown Opcode '{op}' while executing")

    def next_tick(self):
        # fetch
        if self.stage == ControlUnit.Stage.FETCH:
            self.fetch()
            self.stage = ControlUnit.Stage.DECODE
        # decode
        elif self.stage == ControlUnit.Stage.DECODE:
            ended = self.decode()
            if ended:
                self.stage = ControlUnit.Stage.EXECUTE
        # execute
        elif self.stage == ControlUnit.Stage.EXECUTE:
            ended = self.execute()
            if ended:
                self.stage = ControlUnit.Stage.FETCH
        else:
            assert 0, "Unknown ControlUnit stage."
        # increase tick
        self._tick += 1

    def next_instruction(self):
        assert self.stage == ControlUnit.Stage.FETCH, \
            f"Each instruction should start with FETCH, but given '{self.stage}'"

        self.next_tick()
        while self.stage != ControlUnit.Stage.FETCH:
            self.next_tick()

    def __repr__(self) -> str:
        return "TICK: {}, PC: {}, TC: {}, DP: {{ {} }}, IR: {}, STAGE: {}, STAT: {}".format(
            self._tick,
            self.program_counter,
            self.tick_counter,
            self.data_path,
            self.instr_reg,
            self.stage,
            self._status,
        )


def simulation(memory, start_pos, ports: dict[str, dict[str, list[int]]], tick_limit=TICK_LIMIT, by_tick: bool = True):
    data_path = DataPath(memory, ports)
    control_unit = ControlUnit(data_path, start_pos)
    # initial status
    logging.debug('%s', control_unit)

    status = control_unit.get_status()
    while status == ControlUnit.Status.RUNNABLE and control_unit.get_tick() < tick_limit:
        # select execute by tick or by instruction
        if by_tick:
            control_unit.next_tick()
        else:
            control_unit.next_instruction()

        status = control_unit.get_status()
        logging.debug('%s', control_unit)

    if status == ControlUnit.Status.TERMINATED:
        print("WARNING: machine was terminated.")

    return ports


def main(args):
    assert len(args) == 3, "Wrong arguments: machine.py <code_file> <input_file> <output_file>"
    code_file, input_fname, output_fname = args
    
    memory, start_pos = read_code(code_file)
    ports = read_input(input_fname)
    write_output(output_fname, simulation(memory, start_pos, ports))


if __name__ == '__main__':
    logging.getLogger().setLevel(logging.DEBUG)
    main(sys.argv[1:])
