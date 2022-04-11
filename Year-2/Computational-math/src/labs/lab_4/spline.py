import random
from modules.util import ProjectException
from modules.matrix import Matrix

from labs.lab_1.iteration import solve_iterate


A_OFF, B_OFF, C_OFF, D_OFF = 0, 1, 2, 3

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
    matrix_a = _create_matrix_a(points, splines)
    matrix_b = _create_matrix_b(points, splines)
    matrix_x = Matrix().init(matrix_b)
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


def find_point_with_max_error(points, splines):
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



# creating matrix's rows using formula
# matrix a columns:
#    a_0 b_0 c_0 d_0 a_1 b_1 c_1 d_1 ... a_{n - 1} b_{n - 1} c_{n - 1} d_{n - 1}
# matrix_a rows:
#    (1) [1, n - 1]  a_{i - 1} - a_i + b_i h_i - c_i / 2 * h_i^2 + d_i / 6 * h_i^3 = 0
#    (2) [1, n - 1]  b_{i - 1} - b_i + c_i h_i - d_i / 2 * h_i^2                   = 0
#    (3) [1, n - 1]  c_{i - 1} - c_i + d_i h_i                                     = 0
#    (4) [0, n - 1]  a_i                                                           = u(x_i)
#    (5) 0           a_0 - b_0 h_0 + c_0 /2 h_0^2 - d_0 /6 h_0^3                   = u(x_0)
#    (6) 0           c_{n - 1}                                                     = 0       - f(x_n)'' = 0
#    (7) 0           c_0 - d_0 h_0                                                 = 0       - f(x_0)'' = 0
def _create_matrix_a(points, splines):
    n = len(points) - 1
    matrix_a = Matrix(rows=4 * n, columns=4 * n)
    row = 0
    # 1
    for i in range(1, n):
        hi = splines[i].get_hi()
        matrix_a[row][(i - 1) * 4 + A_OFF] = 1
        matrix_a[row][i * 4 + A_OFF] = -1
        matrix_a[row][i * 4 + B_OFF] = hi
        matrix_a[row][i * 4 + C_OFF] = - 1 / 2 * hi ** 2
        matrix_a[row][i * 4 + D_OFF] = 1 / 6 * hi ** 3
        row += 1
    # 2
    for i in range(1, n):
        hi = splines[i].get_hi()
        matrix_a[row][(i - 1) * 4 + B_OFF] = 1
        matrix_a[row][i * 4 + B_OFF] = -1
        matrix_a[row][i * 4 + C_OFF] = hi
        matrix_a[row][i * 4 + D_OFF] = - 1 / 2 * hi ** 2
        row += 1
    # 3
    for i in range(1, n):
        hi = splines[i].get_hi()
        matrix_a[row][(i - 1) * 4 + C_OFF] = 1
        matrix_a[row][i * 4 + C_OFF] = -1
        matrix_a[row][i * 4 + D_OFF] = hi
        row += 1
    # 4
    for i in range(0, n):
        matrix_a[row][i * 4 + A_OFF] = 1
        row += 1
    # 5
    h0 = splines[0].get_hi()
    matrix_a[row][0 + A_OFF] = 1
    matrix_a[row][0 + B_OFF] = - h0
    matrix_a[row][0 + C_OFF] = 1 / 2 * h0 ** 2
    matrix_a[row][0 + D_OFF] = -1 / 6 * h0 ** 3
    row += 1
    # 6
    matrix_a[row][(n - 1) * 4 + C_OFF] = 1
    row += 1
    # 7
    matrix_a[row][0 + C_OFF] = 1
    matrix_a[row][0 + D_OFF] = -h0
    return matrix_a


def _create_matrix_b(points, splines):
    n = len(points) - 1
    matrix_b = Matrix(rows=4 * n, columns=1).init(None)
    row = 0
    # 1, 2, 3
    for _ in range(3 * (n - 1)):
        matrix_b[row][0] = 0
        row += 1
    # 4
    for i in range(n):
        matrix_b[row][0] = points[i]["y"]
        row += 1
    # 5
    matrix_b[row][0] = points[0]["y"]
    row += 1
    # 6
    matrix_b[row][0] = 0
    row += 1
    # 7
    matrix_b[row][0] = 0
    row += 1
    return matrix_b

def get_points_with_error(points, error):
    new_points = list()
    if (error < 0):
        raise ProjectException("invalid error value")
    for p in points:
        new_points.append({"x": p["x"], "y": p["y"] + random.random() * 2 * error - error})
    return new_points
