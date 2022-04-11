import random
from modules.util import ProjectException
from .transformation.simple import create_with_simple_transformations
from .transformation.simple import A_OFF, B_OFF, C_OFF, D_OFF

from labs.lab_1.iteration import solve_iterate


class MultiSpline:
    def __init__(self, splines):
        self.splines = sorted(splines, key=lambda sp: sp.c_point_0)

    def calculate(self, x):
        i = 0
        while (i < len(self.splines) and x >= self.splines[i].c_point_0):
            i += 1
        if (i >= len(self.splines)):
            i -= 1
            if (self.splines[i].c_point_1 >= x):
                return self.splines[i].calculate(x)
            else:
                return None
        else:
            if (i == 0):
                if (self.splines[i].c_point_0 <= x):
                    return self.splines[i].calculate(x)
                else:
                    return None
            else:
                i -= 1
                return self.splines[i].calculate(x)

# END << class Multispline


class Spline:
    def __init__(self, c_point_0, c_point_1):
        self.c_point_0 = c_point_0
        self.c_point_1 = c_point_1
        self.a, self.b, self.c, self.d = None, None, None, None

    def calculate(self, x):
        if (self.a is None or self.b is None or self.c is None or self.d is None):
            raise ValueError("Uninitialized Spline")
        if not (self.c_point_0 <= x <= self.c_point_1):
            raise ValueError("x is out of range [%d %d]" % self.c_point_0 % self.c_point_1)

        return self.a + self.b * (x - self.c_point_1) + \
               self.c / 2 * (x - self.c_point_1) ** 2 + self.d / 6 * (x - self.c_point_1) ** 3

    def get_hi(self):
        return self.c_point_1 - self.c_point_0

    def __str__(self):
        return f"(a: {self.a}, b: {self.b}, c: {self.c}, d: {self.d}, " \
               f"c_point_0: {self.c_point_0}, c_point_1: {self.c_point_1})"

# END << class Spline


def get_splines(points):
    points = sorted(points, key=lambda p: p["x"])
    splines = list()
    # initializing splines with hi
    for i in range(len(points) - 1):
        hi = Spline(points[i]["x"], points[i + 1]["x"])
        if (hi.get_hi() == 0):
            raise ProjectException("has points with same x coordinates")
        splines.append(hi)
    # creating matrices
    matrix_a, matrix_b, matrix_x = create_with_simple_transformations(points, splines)
    # diagonalizing matrix and checking the sufficient condition
    if (not matrix_a.is_convergent()):
        raise ProjectException("matrix_a is not convergent")
    matrix_a = matrix_a.create_diagonal_max_if_can(matrix_b, matrix_x)
    matrix_x = solve_iterate(matrix_a, matrix_b, matrix_x, diff=0.001)
    # filling splines
    for i in range(matrix_x.columns // 4):
        splines[i].a = matrix_x[i * 4 + A_OFF][0]
        splines[i].b = matrix_x[i * 4 + B_OFF][0]
        splines[i].c = matrix_x[i * 4 + C_OFF][0]
        splines[i].d = matrix_x[i * 4 + D_OFF][0]
    return splines


def get_points_with_error(points, error):
    new_points = list()
    if (error < 0):
        raise ProjectException("invalid error value")
    for p in points:
        new_points.append({"x": p["x"], "y": p["y"] + random.random() * 2 * error - error})
    return new_points


def find_point_with_max_error(points, splines: list):
    if (points[0]["x"] != splines[0].c_point_0):
        raise ProjectException("Invalid point coordinates")
    max_error = abs(splines[0].calculate(points[0]["x"]) - points[0]["y"])
    max_index = 0
    for i in range(len(splines)):
        if (points[i]["x"] != splines[i].c_point_1):
            raise ProjectException("Invalid point coordinates")
        else:
            error = abs(splines[i].calculate(points[i + 1]["x"]) - points[i + 1]["y"])
            if (error > max_error):
                max_error = error
                max_index = i + 1
    return max_index
