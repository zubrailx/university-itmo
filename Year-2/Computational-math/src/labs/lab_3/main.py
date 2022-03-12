import json
import sys

from modules.util.color import color_string, Color
from modules.parse import parse
from modules.parse.parse import ParseException
from modules.parse.tokenize import TokenizeException
from modules.util.project_exception import ProjectException
from .simpson import simpson


def solve(input_stream, output_stream):
    data = _read_data_stdin() if input_stream == sys.stdin else _read_data_file(input_stream)
    for e in data:
        try:
            result = simpson(e)
            output_stream.write(json.dumps(result) + "\n")
        except ProjectException as e:
            print(e)


def _read_data_file(input_stream):
    data = json.loads("".join(input_stream.readlines()))
    return data


def _read_data_stdin():
    data = []
    while True:
        data.append(dict())

        data[len(data) - 1]["parse"] = _read_data_stdin_equation()
        range_dict = _read_data_stdin_range()
        data[len(data) - 1]["range_min"] = range_dict["range_min"]
        data[len(data) - 1]["range_max"] = range_dict["range_max"]

        if (input("Finish input? Y/N: ").strip().lower() == "y"):
            break
    return data


def _read_data_stdin_equation():
    data = []
    while True:
        try:
            count = int(input("Enter equation count: "))
            break
        except ValueError:
            print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))

    for _ in range(count):
        while True:
            try:
                equation = parse.parse_expression(input("Enter the parse: "))
                data.append(equation)
                break
            except (ParseException, TokenizeException) as e:
                print(color_string(Color.RED, e))
    return data


def _read_data_stdin_range():
    data = dict()
    while True:
        try:
            range_min = float(input("Enter the left border: "))
            data["range_min"] = range_min
            break
        except ValueError:
            print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
    while True:
        try:
            range_max = float(input("Enter the right border: "))
            data["range_max"] = range_max
            break
        except ValueError:
            print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
    return data
