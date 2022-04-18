import json
import sys

from modules.util import color_string, Color
from modules.parse import parse, ParseException, TokenizeException, CalculationException
from modules.util import ProjectException
from .adams import adams_solve
from .runge_kutta import runge_kutta_solve
from .plot import draw
from labs.lab_4.spline import get_equation


# Solution
def prog_solve(data):
    def flat_equation(e, var_list, dep: str):
        i = var_list.index(dep)
        if (i < 0):
            raise ProjectException("Invalid user input.")
        return lambda x, y: e.calculate({var_list[1 - i]: x, var_list[i]: y})

    x0, y0 = data["initial"]["x"], data["initial"]["y"]
    func = flat_equation(data["equation"], data["var_list"], data["dependant"])
    h, n = data["step"], data["count"]
    real_eq = data["real_eq"]
    points = runge_kutta_solve(x0, y0, func, h, 4)
    points = adams_solve(points, func, h, n)
    equation = get_equation(points)
    draw(points, equation, real_eq)


# User Input
def solve(input_stream, output_stream):
    data = _read_data_stdin() if input_stream == sys.stdin else _read_data_file(input_stream)
    try:
        prog_solve(data)
    except (CalculationException, ProjectException) as e:
        print(color_string(Color.RED, f"ERROR[simpson] >> {e}"))

def _read_data_file(stream_input):
    i_data = json.loads(" ".join(line.strip() for line in stream_input.readlines()))
    p_data = {}
    try:
        p_data["equation"], p_data["var_list"] = parse.parse_expression(i_data["equation"])
        if (len(p_data["var_list"]) != 2):
            raise ParseException("Invalid user input")
        try:
            p_data["real_eq"], t = parse.parse_expression(i_data["real_eq"])
        except KeyError:
            pass
        p_data["dependant"] = i_data["dependant"]
        p_data["step"] = i_data["step"]
        p_data["initial"] = i_data["initial"]
        p_data["count"] = i_data["count"]
    except (ParseException, TokenizeException) as e:
        print(color_string(Color.RED, e))
        print(color_string(Color.RED, f"ERROR >> Invalid user input. Skipping."))
    return p_data

def _read_data_stdin():
    raise ProjectException("User stdin input is not defined.")

def calculate_error(p_real, p_calc):
    errs = []
    for i in range(min(len(p_real), len(p_calc))):
        errs.append(abs(p_real[i]["y"] - p_calc[i]["y"]))
    return errs
