import json
import sys

from modules.util import color_string, Color, ProjectException
from modules.parse import parse_expression, CalculationException, ParseException, TokenizeException

from .spline import get_splines, find_point_with_max_error, get_points_with_error, MultiSpline
from .graph import draw


def solve(ifs, ofs):
    data = _read_data_from_stdin() if ifs == sys.stdin else _read_data_from_ifile(ifs)
    try:
        # get data to draw splines
        p_errs = get_points_with_error(data["points"], data["error"])
        muls_first = MultiSpline(get_splines(p_errs))
        p_err_max_i = find_point_with_max_error(data["points"], muls_first.splines)
        p_errs2 = p_errs.copy()
        p_errs2.pop(p_err_max_i)
        muls_second = MultiSpline(get_splines(p_errs2))
        # draw
        draw(data["points"], p_errs, p_err_max_i, muls_first, muls_second, data["equation"])
    except (CalculationException, ProjectException) as e:
        print(color_string(Color.RED, f"ERROR[simpson] >> {e}"))


def _read_data_from_ifile(ifs):
    data = json.loads(" ".join(line.strip() for line in ifs.readlines()))
    if ("equation" in data.keys()):
        data["equation"], var_list = parse_expression(data["equation"])
        if (len(var_list) > 1):
            raise ProjectException("var_list > 1")
        data["var_list"] = var_list
    return data

def _read_data_from_stdin():
    data = {}
    # reading points
    while True:
        try:
            count = int(input("Enter amount of points to read: "))
            if (count < 0):
                raise ValueError
            break
        except ValueError:
            print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
    point_list = []
    for i in range(count):
        p = {}
        while True:
            try:
                print(f"Enter p[{i + 1}] \"x y\": ", end="")
                p["x"], p["y"] = map(float, input().split())
                break
            except ValueError:
                print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
        point_list.append(p)
    data["points"] = point_list
    # reading equation (optional)
    while True:
        s = input("Enter equation (optional): ").strip()
        if (s is None or s == ""):
            break
        else:
            try:
                equation, var_list = parse_expression(s)
                if (len(var_list) > 1):
                    raise ParseException
                data["equation"], data["var_list"] = equation, var_list
                break
            except (ParseException, TokenizeException):
                print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
    # reading error (optional)
    while True:
        s = input("Enter error (default=0.1): ").strip()
        if (s is None or s == ""):
            break
        else:
            try:
                error = float(s)
                if (error < 0):
                    raise ValueError
                data["error"] = error
                break
            except ValueError:
                print(color_string(Color.RED, "ERROR >> Invalid user input. Try again."))
    return data
