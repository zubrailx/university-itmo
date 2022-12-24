#!/usr/bin/python3

from isa import Opcode, read_code, Instruction

import logging
import sys
from enum import Enum

WORD_SIZE_BITS = 32
WORD_MAX_VALUE = 2 ** (WORD_SIZE_BITS - 1) - 1
WORD_MIN_VALUE = 2 ** (WORD_SIZE_BITS - 1) * (-1)
TICK_LIMIT = 5000


class Alu:
    class Operations(Enum):
        ADD = 0,
        SUB = 1,
        MUL = 2,
        DIV = 3

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
            self.Operations.DIV: lambda l, r: l / r,
        }

    def get_bit(self, flag):
        return self.flags >> flag.value

    def _set_bit(self, x: int, flag):
        n = flag.value
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

    def perform(self, op: Operations, left: int, right: int):
        assert op in self.operations, \
            f"Operation {op} is not present in alu operations."

        res = self.operations[op](left, right)
        res = self._fix_overflow(res)
        self._post_set_flags(res)
        return res


class DataPath(Enum):
    # ports - buses connected to ports where on device side:
    #   ports[i]["in"] - input stack, ports[i]["out"] - output stack
    def __init__(self, memory: list, ports: list[dict]) -> None:
        assert len(memory), "Should be at least one instruction"
        self.memory: list[Instruction] = memory

        self.ports_in: list[int] = [p["in"] for p in ports]
        self.ports_out: list[int] = [p["out"] for p in ports]
        self.accumulator: int = 0
        self.data_reg: int = 0
        self.alu: Alu = Alu()

    def _get_mem_cell(self, addr: int):
        lb = 0
        rb = len(self.memory) - 1
        while lb < rb:
            m = (lb + rb) / 2
            cell = self.memory[addr]
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


class ControlUnit:
    class Status:
        RUNNABLE = 0,
        HALTED = 1,
        TERMINATED = 2

    class Stage:
        FETCH = 0,
        DECODE = 1,
        EXECUTE = 2

    def __init__(self, data_path: DataPath, start_pos: int, tick_limit: int = TICK_LIMIT) -> None:
        self.data_path = data_path
        self.program_counter: int = start_pos
        self.tick_counter: int = 0  # for instruction
        self.instr_reg: Instruction = Instruction()  # struct format
        self.stage = ControlUnit.Stage.FETCH

        self._status = ControlUnit.Status.RUNNABLE
        self._tick: int = 0
        self._tick_limit: int = tick_limit
        # stored value on d triggers in latch
        self._pc_latch: int = -1
        self._tc_latch: int = -1
        self._ac_latch: int = -1
        self._dr_latch: int = -1

    def get_tick(self):
        return self._tick

    def get_status(self):
        return self._status

    # Latch registers
    # is_high - level of signal: False -> 0, True -> 1
    def latch_pc(self, is_high: bool, value=0):
        if is_high:
            self._pc_latch = value
        else:
            self.program_counter = self._pc_latch

    def latch_tc(self, is_high: bool, value=0):
        if is_high:
            self._tc_latch = value
        else:
            self.tick_counter = self._tc_latch

    def latch_ac(self, is_high: bool, value):
        if is_high:
            self._ac_latch = value
        else:
            self.data_path.accumulator = self._ac_latch

    def latch_dr(self, is_high: bool, value):
        if is_high:
            self._dr_latch = value
        else:
            self.data_path.data_reg = self._dr_latch

    # Instruction Cycle
    def fetch(self):
        cmd = self.data_path.memory_perform(self.program_counter, oe=True)
        self.instr_reg = cmd
        self.tick_counter = 0
        self.latch_pc(True, self.program_counter + 1)

    # return value helps program to identify continue to decode or no
    def decode(self) -> bool:
        self.latch_pc(False)

        cmd: Instruction = self.instr_reg
        op = cmd.opcode

        if self.tick_counter == 0:
            self.latch_tc(True, self.tick_counter + 1)
            # indirect commands
            if op in [Opcode.ADD_M, Opcode.SUB_M, Opcode.MOD_M, Opcode.MUL_M,
                      Opcode.LD_M, Opcode.CMP_M]:
                self.data_path.data_reg = cmd.args[0]
                return True
            else:
                return False
        elif self._tc_latch == 1:
            self.latch_tc(False)
            assert self.tick_counter == 1
            self.latch_tc(True, self.tick_counter + 1)
            # load indirect argument
            data = self.data_path.memory_perform(self.program_counter, oe=True).value
            self.latch_dr(True, data)
            return False
        # else
        assert 0, "Decoder should last max 2 ticks"
        return False

    def execute(self) -> bool:
        self.latch_tc(False)
        pass

    def next_tick(self):
        if self.stage == ControlUnit.Stage.FETCH:
            self.fetch()
            self.stage = ControlUnit.Stage.DECODE
        # decode
        elif self.stage == ControlUnit.Stage.DECODE:
            ended = self.execute()
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
        return "SOSTOYANIYA NET, MNE VPADLU"


def simulation(memory, start_pos, ports, tick_limit, by_tick: bool = True):
    data_path = DataPath(memory, ports)
    control_unit = ControlUnit(data_path, start_pos, tick_limit)

    status = control_unit.get_status()
    while status == ControlUnit.Status.RUNNABLE:
        # select execute by tick or by instruction
        if by_tick:
            control_unit.next_tick()
        else:
            control_unit.next_instruction()

        status = control_unit.get_status()
        logging.debug('%s', control_unit)

    if status == ControlUnit.Status.TERMINATED:
        print("WARNING: machine was terminated.")


def main(args):
    assert len(args) == 2, "Wrong arguments: machine.py <code_file> <input_file>"
    code_file, input_file = args

    input_token = []
    memory, start_pos = read_code(code_file)
    print(memory, start_pos)


if __name__ == '__main__':
    logging.getLogger().setLevel(logging.DEBUG)
    main(sys.argv[1:])
