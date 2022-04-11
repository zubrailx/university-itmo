from modules.matrix import Matrix


A_OFF, B_OFF, C_OFF, D_OFF = 0, 1, 2, 3


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

def create_with_simple_transformations(points, splines):
    matrix_a = _create_simple_matrix_a(points, splines)
    matrix_b = _create_simple_matrix_b(points, splines)
    matrix_x = Matrix().init(matrix_b)
    return (matrix_a, matrix_b, matrix_x)

def _create_simple_matrix_a(points, splines):
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


def _create_simple_matrix_b(points, splines):
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
