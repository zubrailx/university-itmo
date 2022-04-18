import json
import numpy
import random
from modules.parse import parse_expression
from modules.util import ProjectException


def main():
    fp = input("Enter file path: ")
    file = open(fp, "w")
    seq = input("Enter equation: ")
    equation, var_list = parse_expression(seq)
    lb, rb = map(float, input("Enter left and right borders: ").split())
    n = int(input("Enter amount of points: "))
    err = float(input("Enter error: "))
    # generating points with error
    st = (rb - lb) / n
    x_arr = list(numpy.arange(lb, rb + st, st))
    y_arr = [equation.calculate({var_list[0]: x}) for x in x_arr]
    points = [{"x": float(x_arr[i]), "y": float(y_arr[i])} for i in range(len(x_arr))]
    points = get_points_with_error(points, err)
    # writing
    obj = dict()
    obj["equation"], obj["points"] = seq, points
    file.write(json.dumps(obj, indent=4) + "\n")
    file.close()
    input("Done...")


def get_points_with_error(points, error):
    new_points = list()
    if (error < 0):
        raise ProjectException("invalid error value")
    for p in points:
        new_points.append({"x": p["x"], "y": p["y"] + random.random() * 2 * error - error})
    return new_points


def find_point_with_max_error(points: list, splines: list):
    if (points[0]["x"] != splines[0].c_point_0):
        raise ProjectException("Invalid point coordinates")
    max_error = abs(splines[0].calculate(points[0]["x"]) - points[0]["y"])
    max_index = 0
    for i in range(len(splines)):
        if (points[i + 1]["x"] != splines[i].c_point_1):
            raise ProjectException("Invalid point coordinates")
        else:
            error = abs(splines[i].calculate(points[i + 1]["x"]) - points[i + 1]["y"])
            if (error > max_error):
                max_error = error
                max_index = i + 1
    return max_index


if __name__ == "__main__":
    main()
