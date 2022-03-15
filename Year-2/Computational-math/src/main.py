import sys
import io
from enum import Enum

import labs as lab

from modules.util import is_word_uint
from modules.util import Color, color_string
from modules.util import ProjectException


class Key(Enum):
    KEY_LAB = "-n"
    KEY_INPUT_DIRECTORY = "-i"
    KEY_OUTPUT_DIRECTORY = "-o"


def is_key(arg: str):
    return arg in [key.value for key in Key]

def is_key_value(key: Key, argv: list):
    key_str = key.value
    return is_key(key_str) and (argv.index(key_str) < len(argv) - 1) and (not is_key(argv[argv.index(key_str) + 1]))

def get_argument_after_key(key: Key, argv: list):
    return argv[sys.argv.index(key.value) + 1]

def is_key_in_arr(key: Key, argv: list):
    return key.value in argv


def get_and_check_key_value(key: Key, argv: list):
    if (is_key_in_arr(key, argv)):
        if (is_key_value(key, argv)):
            return get_argument_after_key(key, sys.argv)
        else:
            raise ProjectException(color_string(Color.RED,
                                                f"FATAL >> For key {key.value} not argument found. Terminating..."))
    return None


def main():
    # disable buffering
    sys.stdout = io.TextIOWrapper(open(sys.stdout.fileno(), "wb", 0), write_through=True)

    lab_number = None
    input_path = None
    output_path = None
    try:
        lab_number = get_and_check_key_value(Key.KEY_LAB, sys.argv)
        input_path = get_and_check_key_value(Key.KEY_INPUT_DIRECTORY, sys.argv)
        output_path = get_and_check_key_value(Key.KEY_OUTPUT_DIRECTORY, sys.argv)
    except ProjectException as e:
        print(e)
        exit()

    input_file = None
    output_file = None
    try:
        if (input_path is not None):
            try:
                input_file = open(input_path, "r")
            except FileNotFoundError:
                raise ProjectException(color_string(Color.RED, "FATAL >> Input file not found. Terminating..."))
        else:
            input_file = sys.stdin
        if (output_path is not None):
            try:
                output_file = open(output_path, "w")
            except FileNotFoundError:
                raise ProjectException(color_string(Color.RED, "FATAL >> Output file not found. Terminating..."))
        else:
            output_file = sys.stdout
    except ProjectException as e:
        print(e)
        exit()

    try:
        if (lab_number is not None and is_word_uint(lab_number)):
            lab_number = int(lab_number)
            lab.solve(lab_number, input_file, output_file)
        else:
            raise ProjectException(color_string(Color.RED, "FATAL >> Lab number is not present or is not integer. " 
                                                           "Terminating..."))
    except ProjectException as e:
        print(e)
        exit()
    return


if __name__ == "__main__":
    main()
