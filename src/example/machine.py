#!/usr/bin/python3
# pylint: disable=missing-function-docstring  # чтобы не быть Капитаном Очевидностью
# pylint: disable=invalid-name                # сохраним традиционные наименования сигналов
# pylint: disable=consider-using-f-string     # избыточный синтаксис

"""Модель процессора, позволяющая выполнить странслированные программы на языке Brainfuck.
"""
import logging
import sys


from isa import Opcode, read_code


class DataPath():
    """Тракт данных (пассивный), включая: ввод/вывод, память и арифметику.

                     +--------------+  addr   +--------+
               +---->| data_address |---+---->|  data  |
               |     +--------------+   |     | memory |
           +-------+                    |     |        |
    sel -->|  MUX  |         +----------+     |        |
           +-------+         |                |        |
            ^     ^          |                |        |
            |     |          |        data_in |        | data_out
            |     +---(+1)---+          +---->|        |-----+
            |                |          |     |        |     |
            +---------(-1)---+          |  oe |        |     |
                                        | --->|        |     |
                                        |     |        |     |
                                        |  wr |        |     |
                                        | --->|        |     |
                                        |     +--------+     |
                                        |                    v
                                    +--------+     latch  +-----+
                          sel ----> |  MUX   |    ------->| acc |
                                    +--------+            +-----+
                                     ^   ^  ^                |
                                     |   |  |                +---(==0)---> zero
                                     |   |  |                |
                                     |   |  +---(+1)---------+
                                     |   |                   |
                                     |   +------(-1)---------+
                                     |                       |
            input -------------------+                       +---------> output


    - data_memory -- однопортовая, поэтому либо читаем, либо пишем.
    - input/output -- токенизированная логика ввода-вывода. Не детализируется.
    - input -- может вызвать остановку процесса моделирования,
      если буфер входных значений закончился.

    Реализованные методы соответствуют группам активированных сигналов.
    Сигнал "исполняется" за один такт. Корректность использования сигналов за
    один такт -- задача ControlUnit.
    """

    def __init__(self, data_memory_size, input_buffer):
        assert data_memory_size > 0, "Data_memory size should be non-zero"
        self.data_memory_size = data_memory_size
        self.data_memory = [0] * data_memory_size
        self.data_address = 0
        self.acc = 0
        self.input_buffer = input_buffer
        self.output_buffer = []

    def latch_data_addr(self, sel):
        assert sel in {Opcode.LEFT.value, Opcode.RIGHT.value}, \
            "internal error, incorrect selector: {}".format(sel)

        if sel == Opcode.LEFT.value:
            self.data_address -= 1
        elif sel == Opcode.RIGHT.value:
            self.data_address += 1

        assert 0 <= self.data_address < self.data_memory_size, \
            "out of memory: {}".format(self.data_address)

    def latch_acc(self):
        """Вывести слово из памяти (oe от Output Enable) и защёлкнуть его в аккумулятор."""
        self.acc = self.data_memory[self.data_address]

    def output(self):
        """oe от Output Enable. Вывести слово на порт ввода/вывода.

        Вывод:

        - производится через ASCII-символы по спецификации;
        - вывод осуществляется просто в буфер.
        """
        symbol = chr(self.acc)
        logging.debug('output: %s << %s', repr(
            ''.join(self.output_buffer)), repr(symbol))
        self.output_buffer.append(symbol)

    def wr(self, sel):
        """wr (от WRite), сохранить в память."""
        assert sel in {Opcode.INC.value, Opcode.DEC.value, Opcode.INPUT.value}, \
            "internal error, incorrect selector: {}".format(sel)

        if sel == Opcode.INC.value:
            self.data_memory[self.data_address] = self.acc + 1
            if self.data_memory[self.data_address] == 128:
                self.data_memory[self.data_address] = -128
        elif sel == Opcode.DEC.value:
            self.data_memory[self.data_address] = self.acc - 1
            if self.data_memory[self.data_address] == -129:
                self.data_memory[self.data_address] = 127
        elif sel == Opcode.INPUT.value:
            if len(self.input_buffer) == 0:
                raise EOFError()
            symbol = self.input_buffer.pop(0)
            symbol_code = ord(symbol)
            assert -128 <= symbol_code <= 127, \
                "input token is out of bound: {}".format(symbol_code)
            self.data_memory[self.data_address] = symbol_code
            logging.debug('input: %s', repr(symbol))

    def zero(self):
        """Флаг"""
        return self.acc == 0


class ControlUnit():
    """Блок управления процессора. Выполняет декодирование инструкций и
    управляет состоянием процессора, включая обработку данных (DataPath).

    Считается, что любая инструкция может быть в одно слово. Следовательно,
    индекс памяти команд эквивалентен номеру инструкции.

   +------------------(+1)-------+
   |                             |
   |   +-----+                   |
   +-->|     |     +---------+   |    +---------+
       | MUX |---->| program |---+--->| program |
   +-->|     |     | counter |        | memory  |
   |   +-----+     +---------+        +---------+
   |      ^                               |
   |      | sel_next                      | current instruction
   |      |                               |
   +---------------(select-arg)-----------+
          |                               |      +---------+
          |                               |      |  step   |
          |                               |  +---| counter |
          |                               |  |   +---------+
          |                               v  v        ^
          |                       +-------------+     |
          +-----------------------| instruction |-----+
                      +---------->| decoder     |
                      |           +-------------+
                      |                   |
                      |                   | signals
                      |                   v
                      |    zero     +----------+
                      +-------------|          |
                                    | DataPath |
                     input -------->|          |----------> output
                                    +----------+
    """

    def __init__(self, program, data_path):
        self.program = program
        self.program_counter = 0
        self.data_path = data_path
        self._tick = 0

    def tick(self):
        """Счётчик тактов процессора. Вызывается при переходе на следующий такт."""
        self._tick += 1

    def current_tick(self):
        return self._tick

    def latch_program_counter(self, sel_next):
        if sel_next:
            self.program_counter += 1
        else:
            instr = self.program[self.program_counter]
            assert 'arg' in instr, "internal error"
            self.program_counter = instr["arg"]

    def decode_and_execute_instruction(self):
        instr = self.program[self.program_counter]
        opcode = instr["opcode"]

        if opcode is Opcode.HALT:
            raise StopIteration()

        if opcode is Opcode.JMP:
            addr = instr["arg"]
            self.program_counter = addr
            self.tick()

        elif opcode is Opcode.JZ:
            addr = instr["arg"]

            self.data_path.latch_acc()
            self.tick()

            if self.data_path.zero():
                self.latch_program_counter(sel_next=False)
            else:
                self.latch_program_counter(sel_next=True)
            self.tick()
        else:
            if opcode in {Opcode.RIGHT, Opcode.LEFT}:
                self.data_path.latch_data_addr(opcode.value)
                self.latch_program_counter(sel_next=True)
                self.tick()

            elif opcode in {Opcode.INC, Opcode.DEC, Opcode.INPUT}:
                self.data_path.latch_acc()
                self.tick()

                self.data_path.wr(opcode.value)
                self.latch_program_counter(sel_next=True)
                self.tick()

            elif opcode is Opcode.PRINT:
                self.data_path.latch_acc()
                self.tick()

                self.data_path.output()
                self.latch_program_counter(sel_next=True)
                self.tick()

    def __repr__(self):
        state = "{{TICK: {}, PC: {}, ADDR: {}, OUT: {}, ACC: {}}}".format(
            self._tick,
            self.program_counter,
            self.data_path.data_address,
            self.data_path.data_memory[self.data_path.data_address],
            self.data_path.acc,
        )

        instr = self.program[self.program_counter]
        opcode = instr["opcode"]
        arg = instr.get("arg", "")
        term = instr["term"]
        action = "{} {} ('{}' @ {}:{})".format(opcode, arg, term.symbol, term.line, term.pos)

        return "{} {}".format(state, action)


def simulation(code, input_tokens, data_memory_size, limit):
    """Запуск симуляции процессора.

    Длительность моделирования ограничена количеством выполненных инструкций.
    """
    data_path = DataPath(data_memory_size, input_tokens)
    control_unit = ControlUnit(code, data_path)
    instr_counter = 0

    logging.debug('%s', control_unit)
    try:
        while True:
            assert limit > instr_counter, "too long execution, increase limit!"
            control_unit.decode_and_execute_instruction()
            instr_counter += 1
            logging.debug('%s', control_unit)
    except EOFError:
        logging.warning('Input buffer is empty!')
    except StopIteration:
        pass
    logging.info('output_buffer: %s', repr(''.join(data_path.output_buffer)))
    return ''.join(data_path.output_buffer), instr_counter, control_unit.current_tick()


def main(args):
    assert len(args) == 2, "Wrong arguments: machine.py <code_file> <input_file>"
    code_file, input_file = args

    code = read_code(code_file)
    with open(input_file, encoding="utf-8") as file:
        input_text = file.read()
        input_token = []
        for char in input_text:
            input_token.append(char)

    output, instr_counter, ticks = simulation(code,
                                              input_tokens=input_token,
                                              data_memory_size=100, limit=1000)

    print(''.join(output))
    print("instr_counter: ", instr_counter, "ticks:", ticks)


if __name__ == '__main__':
    logging.getLogger().setLevel(logging.DEBUG)
    main(sys.argv[1:])
