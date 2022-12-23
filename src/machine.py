#!/usr/bin/python3

from isa import Opcode, read_code
    
"""Модуль, эмулирующий процессор"""

import logging
import sys
from enum import Enum

def main(args):
    assert len(args) == 2, "Wrong arguments: machine.py <code_file> <input_file>"
    code_file, input_file = args

    input_token = []
    memory = read_code(code_file)
    print(memory)

if __name__ == '__main__':
    logging.getLogger().setLevel(logging.DEBUG)
    main(sys.argv[1:])

