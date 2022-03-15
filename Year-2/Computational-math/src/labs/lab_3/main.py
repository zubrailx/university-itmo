import json
import sys

from modules.util import color_string, Color
from modules.parse import parse, ParseException, TokenizeException
from modules.util import ProjectException
from .simpson import simpson


def solve(input_stream, output_stream):
    data = _read_data_stdin() if input_stream == sys.stdin else _read_data_file(input_stream)
    for e in data:
        try:
            result = simpson(e)
            if output_stream == sys.stdout:
                _print_result(result)
            else:
                output_stream.write(json.dumps(result, indent=4) + "\n")
        except ProjectException as e:
            print(color_string(Color.RED, f"ERROR[simpson] >> {e}"))


def _print_result(result):
    for i in range(len(result)):
        print(f"Equation #{i + 1}:")
        print("\tIntegral:\t%.8f:" % result[i]['result'])
        print("\tError R1:\t%.8e:" % result[i]['r1'])
        print("\tError R2:\t%.8e:" % result[i]['r2'])
        print("\tError R:\t%.8e:" % result[i]['r'])


def _read_data_file(stream_input):
    data = json.loads(" ".join(line.strip() for line in stream_input.readlines()))
    for i in range(len(data)):
        try:
            equation_list = data[i]["equation"]
            var_lst = set()
            data[i]["equation"] = list()
            for j in range(len(equation_list)):
                equation, var_cur_lst = parse.parse_expression(equation_list[j])
                data[i]["equation"].append((equation, var_cur_lst))
                var_lst |= set(var_cur_lst)
            data[i]["var_lst"] = list(var_lst)
        except ParseException as e:
            print(color_string(Color.RED, e))
            print(color_string(Color.RED, f"ERROR >> Invalid user input in index {i} method in list. Skipping."))
            data.pop(i)
    return data


def _read_data_stdin():
    data = []
    while True:
        data.append(dict())

        data[len(data) - 1]["equation"], data[len(data) - 1]["var_list"] = _read_data_stdin_equation()
        range_dict = _read_data_stdin_range()
        data[len(data) - 1]["data"] = dict()
        data[len(data) - 1]["data"]["range_min"] = range_dict["range_min"]
        data[len(data) - 1]["data"]["range_max"] = range_dict["range_max"]
        data[len(data) - 1]["data"]["step"] = _read_data_stdin_step()

        if (input("Finish input? Y/N: ").strip().lower() == "y"):
            break
    return data


def _read_data_stdin_equation():
    data = []
    count = 1
    var_list = set()
    for i in range(count):
        while True:
            try:
                equation, var_list_cur = parse.parse_expression(input(f"Enter the equation [{i + 1}]: "))
                var_list |= set(var_list_cur)
                data.append((equation, var_list_cur))
                break
            except (ParseException, TokenizeException) as e:
                print(color_string(Color.RED, e))
    return (data, var_list)


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


def _read_data_stdin_step():
    while True:
        try:
            step = float(input("Enter step: "))
            return step
        except ValueError:
            print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
