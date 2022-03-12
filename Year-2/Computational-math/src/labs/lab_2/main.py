import json
import sys

from . import methods
from modules.util.color import color_string, Color
from modules.util.project_exception import ProjectException
from modules.parse import parse
from modules.parse.parse import ParseException


method_dict = {
    "split_half":       (methods.split_half,        "Splitting in half"),
    "tangent":          (methods.tangent,           "Tangent method"),
    "simple_iteration": (methods.simple_iteration,  "Simple iteration method (for systems)")
}


def solve(stream_input, stream_output):
    method_list = _get_data_from_stdin() if stream_input == sys.stdin else _get_data_from_file(stream_input)
    for m in method_list:
        if not (m["method"] in method_dict.keys()):
            print(color_string(Color.RED, f"ERROR >> Method '{m['method']}' not found."))
        try:
            # TODO parse output to user stdout (global function, not only for lab 2)
            result = method_dict[m["method"]][0](m)
            stream_output.write(json.dumps(result) + "\n")
        except ProjectException as e:
            print(e)
    return


def _get_data_from_file(stream_input):
    data = json.loads(" ".join(line.strip() for line in stream_input.readlines()))
    for i in range(len(data)):
        try:
            equation_list = data[i]["parse"]
            for j in range(len(equation_list)):
                data[i]["parse"][j] = parse.parse_expression(equation_list[j])
        except ParseException as e:
            print(color_string(Color.RED, e))
            print(color_string(Color.RED, f"ERROR >> Invalid user input in index {i} method in list. Skipping."))
            data.pop(i)
    return data


def _get_data_from_stdin():
    data = []
    while True:
        data.append(dict())
        data[len(data) - 1]["method"] = _get_data_from_stdin_method()
        method = method_dict[data[len(data) - 1]["method"]][0]
        data[len(data) - 1]["parse"], var_list = _get_data_from_stdin_equation(method)
        data[len(data) - 1]["data"] = _get_data_from_stdin_data(var_list, method)
        data[len(data) - 1]["var_list"] = var_list
        if (input("Finish input? Y/N: ").strip().lower() == "y"):
            break
    return data


def _get_data_from_stdin_method():
    method_dict_keys = list(method_dict.keys())
    for i in range(len(method_dict_keys)):
        print(f"{i + 1}) {method_dict[method_dict_keys[i]][1]}")
    while True:
        try:
            number = int(input("Choose the method number: ")) - 1
            if not (0 <= number < len(method_dict_keys)):
                raise ValueError()
            break
        except ValueError:
            print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
    return method_dict_keys[number]


def _get_data_from_stdin_equation(function):
    var_set = set()
    equation_list = list()

    if (function == methods.simple_iteration):
        while True:
            try:
                count = int(input("Enter the number of equations: "))
                if (count <= 0):
                    raise ValueError()
                break
            except ValueError:
                print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
    else:
        count = 1

    for _ in range(count):
        while True:
            try:
                node, var_list = parse.parse_expression(input("Equation: ").strip())
                equation_list.append(node)
                current_set = var_set | set(var_list)
                if (len(current_set) > count):
                    raise ValueError("Too much variables, try again.")
                var_set = current_set
                break
            except ParseException as e:
                print(color_string(Color.RED, e))
                print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
            except ValueError as e:
                print(e)
    return (equation_list, list(var_set))


def _get_data_from_stdin_data(var_list: list, function):
    _get_data_from_stdin_data_description()
    data = {}
    # iterations
    while True:
        try:
            iterations = input("Enter 'iterations' or blank: ")
            if (iterations.strip() == ''):
                break
            iterations = int(iterations)
            if (iterations < 0):
                raise ValueError()
            data["iterations"] = iterations
            break
        except ValueError:
            print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
    # range_min
    while True:
        try:
            range_min = input("Enter 'range_min' or blank: ")
            if (range_min.strip() == ''):
                break
            range_min = float(range_min)
            data["range_min"] = range_min
            break
        except ValueError:
            print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
    # range_max
    while True:
        try:
            range_max = input("Enter 'range_max' or blank: ")
            if (range_max.strip() == ''):
                break
            range_max = float(range_max)
            data["range_max"] = range_max
            break
        except ValueError:
            print(color_string(Color.RED, "ERROR >> Invalid user input"))
    # x_0
    if (function == methods.tangent or function == methods.simple_iteration):
        for i in range(len(var_list)):
            while True:
                try:
                    var_value = float(input(f"Enter the value for '{var_list[i]}: '"))
                    data[var_list[i]] = var_value
                    break
                except ValueError:
                    print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
    return data


def _get_data_from_stdin_data_description():
    print("Additional arguments: ")
    print("-" * 100)
    print("iterations   (int)   - count of iterations for the method")
    print("range_min    (float) - minimum value of border")
    print("range_max    (float) - maximum value of border")
    print("x_0          (dict<variable(str), float>) - values of arguments for first computation")
    print("-" * 100)
    print()
