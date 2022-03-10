import json
import sys

from . import methods
from modules.util.color import color_string, Color
from modules.util.project_exception import ProjectException


method_dict = {
    "split_half":       methods.split_half,
    "tangent":          methods.tangent,
    "simple_iteration": methods.simple_iteration
}


def solve(stream_input, stream_output):
    method_list = _get_data_from_stdin() if stream_input == sys.stdin else _get_data_from_file(stream_input)
    for m in method_list:
        if not (m["method"] in method_dict.keys()):
            print(color_string(Color.RED, f"ERROR >> Method '{m['method']}' not found."))
        # can be parsed
        try:
            result = method_dict[m["method"]](m)
            stream_output.write(json.dumps(result))
            stream_output.write("\n")
        except ProjectException as e:
            print(e)
    return

def _get_data_from_file(stream_input):
    return json.loads(" ".join(line.strip() for line in stream_input.readlines()))


methods_stdio_name = {
    1: ("Splitting in half", "split_half"),
    2: ("Tangent method", "tangent"),
    3: ("Simple iteration method (for systems)", "simple_iteration")
}

def _get_data_from_stdin():
    data = []
    print("Beginning the user input...")
    for k, v in methods_stdio_name.items():
        print(f"{k}) {v[0]}")
    current = 0
    while True:
        data.append(dict())
        while True:
            try:
                number = int(input("Choose the method number: "))
                if number not in methods_stdio_name.keys():
                    raise ValueError()
                break
            except ValueError:
                print(color_string(Color.RED, "ERROR >> Invalid user input"))
        data[current]["method"] = methods_stdio_name[number][1]

        while True:
            try:
                count = int(input("Enter the number of equations: "))
                if number not in methods_stdio_name.keys():
                    raise ValueError()
                break
            except ValueError:
                print(color_string(Color.RED, "ERROR >> Invalid user input"))
        data[current]["equation"] = []
        for _ in range(count):
            equation = input("Equation: ")
            data[current]["equation"].append(equation)

        _print_additional_description()
        while True:
            try:
                additional = []
                line = sys.stdin.readline().strip()
                while line:
                    additional.append(line)
                    if line:
                        line = sys.stdin.readline().strip()
                    else:
                        break
                additional_json = json.loads(" ".join(additional))
                data[current]["data"] = additional_json
                break
            except Exception:
                print(color_string(Color.RED, "Invalid user input. Try again."))

        if (input("Do you want to end? y/n: ").strip().lower() == "y"):
            break
    return data

def _print_additional_description():
    print("Enter additional information in JSON format, cuz there are a lot of arguments :")
    print("Example format: ")
    print("-------------------------")
    print('{')
    print('"iterations": 10,')
    print('"x_0": {')
    print('"x_1": 0.9,')
    print('"x_2": 0.9')
    print('},')
    print('"range_min": -3,')
    print('"range_max": 2')
    print('}')
    print("-------------------------")
    print("Press Ctrl + D to end input")

