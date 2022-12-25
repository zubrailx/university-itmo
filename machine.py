#!/usr/bin/python3
# pylint: disable=missing-function-docstring  # чтобы не быть Капитаном Очевидностью
# pylint: disable=invalid-name                # сохраним традиционные наименования сигналов
# pylint: disable=consider-using-f-string     # избыточный синтаксис
# pylint: disable=too-few-public-methods
# pylint: disable=too-many-instance-attributes
# pylint: disable=too-many-return-statements
# pylint: disable=too-many-statements

"""Модель процессора, позволяющая выполнить странслированные программы на языке Assembly.
"""

from enum import Enum
import sys
import logging
from typing import TypeVar, Generic, Any, Callable, Optional

from isa import Opcode, read_code, Instruction, WORD_WIDTH, read_input, \
    write_output, WORD_MAX_VALUE, WORD_MIN_VALUE

T = TypeVar('T')
Ports = dict[int, dict[str, list[int]]]


TICK_LIMIT = 10000
ITOC_CONST = ord('0')
INT_UNDEF = -1


# Utility
# Wrapper to pass to methods values by reference
class Wrapper(Generic[T]):
    """ Wrapper to different types to pass values like 'by reference'."""

    def __init__(self, val: T) -> None:
        self.val: T = val

    def get(self) -> T:
        return self.val

    def set(self, val: T) -> None:
        self.val = val

    def __eq__(self, o: Any) -> bool:
        if isinstance(self, type(o)):
            return bool(self.val == o.val)
        if isinstance(self.val, type(o)):
            return bool(self.val == o)
        assert 0, f"Equal operator doesn't support operands '{type(self)}' '{type(o)}'"
        return False

    def __repr__(self) -> str:
        return self.val.__repr__()


class Latch(Generic[T]):
    """Trigger to pass value on if state_prev == High and state_cur == Low."""

    # target - wrapper, because can't pass value by pointer
    def __init__(self, target: Wrapper[T]):
        self.latched: T | None = None
        self.was_high: bool = False
        self.target: Wrapper[T] = target

    # if False then pass value to latch else
    def latch(self, is_high: bool, value: T | None = None) -> None:
        if self.was_high and not is_high:
            assert self.latched is not None
            self.target.set(self.latched)
        else:
            self.latched = value
        # store last state
        self.was_high = is_high


class Alu:
    """Арифметическое логическое устройство, которое может даже делить."""

    class Operations(Enum):
        """Тип операции АЛУ."""
        ADD = 0
        SUB = 1
        MUL = 2
        DIV = 3
        MOD = 4
        RIGHT = 5
        LEFT = 6

    class Flags(int, Enum):
        """Флаги, получаемые после подсчета АЛУ."""
        # idx of bit
        ZERO = 0
        NEG = 1

    def __init__(self) -> None:
        self.flags: int = 0b0
        self.operations: dict[Alu.Operations, Callable[[int, int], int]] = {
            self.Operations.ADD: lambda lsv, rsv: lsv + rsv,
            self.Operations.SUB: lambda lsv, rsv: lsv - rsv,
            self.Operations.MUL: lambda lsv, rsv: lsv * rsv,
            self.Operations.DIV: lambda lsv, rsv: lsv // rsv,
            self.Operations.MOD: lambda lsv, rsv: lsv % rsv,
            self.Operations.RIGHT: lambda _, rsv: rsv,
            self.Operations.LEFT: lambda lsv, _: lsv
        }

    def get_bit(self, flag: Flags) -> int:
        return int(self.flags >> flag.value) & 0x1

    def _set_bit(self, x: int, flag: Flags) -> None:
        n: int = flag.value
        self.flags = self.flags & ~(1 << n) | (x << n)

    def _post_set_flags(self, res: int) -> None:
        self._set_bit(res == 0, Alu.Flags.ZERO)
        self._set_bit(res < 0, Alu.Flags.NEG)

    @staticmethod
    def _fix_overflow(res: int) -> int:
        if res > WORD_MAX_VALUE:
            return int(res + WORD_MIN_VALUE + WORD_MAX_VALUE)
        if res < WORD_MIN_VALUE:
            return int(res + WORD_MAX_VALUE - WORD_MIN_VALUE)
        return res

    def perform(self, op: Operations, left: int, right: int) -> int:
        assert op in self.operations, \
            f"Operation {op} is not present in alu operations."

        res = self.operations[op](left, right)
        res = self._fix_overflow(res)
        self._post_set_flags(res)
        return res


class DataPath:
    """Тракт данных (пассивный), включая: ввод/вывод, память и арифметику."""

    # ports - buses connected to ports where on device side
    def __init__(self, memory: list[Instruction], ports: Ports) -> None:
        assert len(memory), "Should be at least one instruction"
        self.memory: list[Instruction] = memory

        self.ports_in: dict[int, list[int]] = {}
        self.ports_out: dict[int, list[int]] = {}
        for num, port in ports.items():
            if "in" in port:
                self.ports_in[num] = port["in"]
            if "out" in port:
                self.ports_out[num] = port["out"]

        self.alu: Alu = Alu()
        # registers
        self.ac: Wrapper[int] = Wrapper[int](INT_UNDEF)  # accumulator
        self.dr: Wrapper[int] = Wrapper[int](INT_UNDEF)  # data register

    def _get_mem_cell(self, addr: int) -> Instruction:
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

    def memory_perform(self, addr: int, data_in: int = 0, oe: bool = False,
                       wr: bool = False) -> Optional[Instruction]:
        cell = self._get_mem_cell(addr)
        if wr:
            if cell.value is not None:
                cell.value = data_in
                logging.debug("mem[%d] = %d", addr, data_in)
            else:
                raise Exception(f"DataPath: Trying to overwrite code on address '{addr}")
        if oe:
            return cell
        assert oe or wr, "Invalid arguments to memory_perform. Should be oe=True or wr=True"
        return None

    def alu_perform(self, op: Alu.Operations, left: int, right: int) -> int:
        return self.alu.perform(op, left, right)

    # io_sel - number of port
    def io_perform(self, io_sel: int, is_in: bool=False, is_out: bool=False) -> int:
        assert not is_in or not is_out, "In and Out are set to True, undefined behaviour"
        if is_in:
            if len(self.ports_in[io_sel]) == 0:
                raise Exception(f"DataPath: ports[{io_sel}].in queue is empty")
            pin = self.ports_in[io_sel].pop(0)
            logging.debug("ports[%d].in >> %d ('%s')", io_sel, pin, chr(pin))
            return pin
        if is_out:
            pout = self.ac.get()
            logging.debug("ports[%d].out << %d ('%s')", io_sel, pout, chr(pout))
            self.ports_out[io_sel].append(pout)
        return INT_UNDEF

    def __repr__(self) -> str:
        return "AC: {}, DR: {}, FLG: {{ Z: {}, N: {} }}".format(
            self.ac,
            self.dr,
            self.alu.get_bit(Alu.Flags.ZERO),
            self.alu.get_bit(Alu.Flags.NEG)
        )


class ControlUnit:
    """
    Блок управления процессора. Выполняет декодирование инструкций и
    управляет состоянием процессора, включая обработку данных (DataPath).
    """
    class Status(Enum):
        """Статус машины."""
        RUNNABLE = 0
        HALTED = 1
        TERMINATED = 2

    class Stage(Enum):
        """Этап выполнения команды."""
        FETCH = 0
        DECODE = 1
        EXECUTE = 2

    def __init__(self, data_path: DataPath, start_pos: int) -> None:
        self.data_path: DataPath = data_path
        # registers
        self.program_counter: Wrapper[int] = Wrapper[int](start_pos)
        self.tick_counter: Wrapper[int] = Wrapper[int](INT_UNDEF)  # for instruction
        self.instr_reg: Instruction = Instruction()  # struct format

        self.stage: ControlUnit.Stage = ControlUnit.Stage.FETCH

        self._status = ControlUnit.Status.RUNNABLE
        self._tick: int = 0
        # to emulate latch
        self.pc_latch: Latch[int] = Latch[int](self.program_counter)
        self.tc_latch: Latch[int] = Latch[int](self.tick_counter)
        self.ac_latch: Latch[int] = Latch[int](self.data_path.ac)
        self.dr_latch: Latch[int] = Latch[int](self.data_path.dr)

    def get_tick(self) -> int:
        return self._tick

    def get_status(self) -> Status:
        return self._status

    # Instruction Cycle
    def fetch(self) -> bool:
        # unlatch accumulator,and program_counter
        self.ac_latch.latch(False)
        self.pc_latch.latch(False)

        cmd = self.data_path.memory_perform(self.program_counter.get(), oe=True)
        assert isinstance(cmd, Instruction)
        self.instr_reg = cmd

        self.tc_latch.latch(True, 0)
        self.pc_latch.latch(True, self.program_counter.get() + WORD_WIDTH)
        return True

    # cycle ended or not
    def decode(self) -> bool:
        self.pc_latch.latch(False)
        self.tc_latch.latch(False)
        self.dr_latch.latch(False)
        self.tc_latch.latch(True, self.tick_counter.get() + 1)

        cmd: Instruction = self.instr_reg
        op = cmd.opcode
        # we are not iterating over data
        if cmd.args is None:
            raise Exception(f"Trying to iterate over data '{cmd}'")

        if self.tick_counter.get() == 0:
            # commands with at least one argument
            if op not in [Opcode.INC, Opcode.DEC, Opcode.CTOI, Opcode.ITOC, Opcode.HALT]:
                self.dr_latch.latch(True, cmd.args[0])
            # indirect commands
            return op not in [Opcode.ADD_M, Opcode.SUB_M, Opcode.MOD_M, Opcode.MUL_M,
                              Opcode.LD_M, Opcode.CMP_M]

        if self.tick_counter.get() == 1:
            # load indirect argument
            instr = self.data_path.memory_perform(self.data_path.dr.get(), oe=True)
            assert isinstance(instr, Instruction)
            if instr.value is None:
                raise Exception(f"Trying to load instruction '{instr}' instead of variable")

            self.dr_latch.latch(True, instr.value)
            return True

        assert 0, f"Tick number is invalid: {self.tick_counter.get()}"
        return True

    # cycle ended or not
    def execute(self) -> bool:
        self.tc_latch.latch(False)
        self.dr_latch.latch(False)
        self.tc_latch.latch(True, self.tick_counter.get() + 1)
        cmd: Instruction = self.instr_reg
        op = cmd.opcode

        if op == Opcode.HALT:
            self._status = ControlUnit.Status.HALTED
            return True

        # Accumulator modification commands
        if op in [Opcode.INC, Opcode.DEC, Opcode.ITOC, Opcode.CTOI]:
            ac_old: int = self.data_path.ac.get()
            ac_new: int = INT_UNDEF
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
            ac_old = self.data_path.ac.get()
            ac_new = INT_UNDEF
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
            ac_val = self.data_path.alu_perform(Alu.Operations.RIGHT, 0, dr_val)
            self.ac_latch.latch(True, ac_val)
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
                self.pc_latch.latch(True, self.data_path.dr.get())
            return True

        # Port mapped IO
        if op in [Opcode.IN_IMM, Opcode.OUT_IMM]:
            io_sel = self.data_path.dr.get()
            if op == Opcode.IN_IMM:
                self.ac_latch.latch(True, self.data_path.io_perform(io_sel, is_in=True))
            else:
                self.data_path.io_perform(io_sel, is_out=True)
            return True

        raise Exception(f"Reached unknown Opcode '{op}' while executing")

    def next_tick(self) -> None:
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

    def next_instruction(self) -> None:
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


def simulation(memory: list[Instruction], start_pos: int, ports: Ports,
               tick_limit: int = TICK_LIMIT, by_tick: bool = True) -> Ports:
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


def main(args: list[str]) -> None:
    assert len(args) == 3, "Wrong arguments: machine.py <code_file> <input_file> <output_file>"
    code_file, input_fname, output_fname = args

    memory, start_pos = read_code(code_file)
    ports = read_input(input_fname)
    ports_out = simulation(memory, start_pos, ports, by_tick=False)
    write_output(output_fname, ports_out)


if __name__ == '__main__':
    logging.getLogger().setLevel(logging.DEBUG)
    main(sys.argv[1:])
