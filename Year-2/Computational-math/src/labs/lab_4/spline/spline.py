import random
from modules.util import ProjectException
from .transformation.simple import create_with_simple_transformations, fill_simple_transformed_splines
from .transformation.triagonal import create_with_triagonal_transformations, fill_triagonal_transformed_splines

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

# END << class MultiSpline


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
    splines = []
    # initializing splines with hi
    for i in range(len(points) - 1):
        hi = Spline(points[i]["x"], points[i + 1]["x"])
        if (hi.get_hi() == 0):
            raise ProjectException("has points with same x coordinates")
        splines.append(hi)
    # creating matrices
    # matrix_a, matrix_b, matrix_x = create_with_simple_transformations(points, splines)
    matrix_a, matrix_b, matrix_x = create_with_triagonal_transformations(points, splines)
    # diagonalizing matrix, checking condition and solving
    if (not matrix_a.is_convergent()):
        raise ProjectException("matrix_a is not convergent")
    matrix_a = matrix_a.create_diagonal_max_if_can(matrix_b, matrix_x)[0]
    matrix_x = solve_iterate(matrix_a, matrix_b, matrix_x, diff=0.001)[0]
    # filling splines
    # fill_simple_transformed_splines(matrix_x, splines)
    fill_triagonal_transformed_splines(matrix_x, splines, points)
    return splines

def get_equation(points):
    return MultiSpline(get_splines(points))
