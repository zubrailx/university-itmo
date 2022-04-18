from modules.matrix import Matrix


def create_with_triagonal_transformations(points, splines):
    matrix_a = _create_matrix_a(splines)
    matrix_b = _create_matrix_b(points)
    matrix_x = Matrix().init(matrix_b)
    return (matrix_a, matrix_b, matrix_x)


def fill_triagonal_transformed_splines(matrix_x, splines, sorted_points):
    c_arr = [0]
    for i in range(matrix_x.rows):
        c_arr.append(matrix_x[i][0])
    c_arr.append(0)
    for i in range(len(splines)):
        splines[i].a = _get_ai(sorted_points, i + 1)
        splines[i].b = _get_bi(sorted_points, i + 1, c_arr)
        splines[i].c = c_arr[i + 1]
        splines[i].d = _get_di(sorted_points, i + 1, c_arr)
    return


def _create_matrix_a(splines: list):
    n = len(splines)
    matrix_a = Matrix(rows=n - 1, columns=n - 1)
    if (matrix_a.rows > 1):
        matrix_a[0][0], matrix_a[0][1] = 2, (splines[1].get_hi() / (splines[0].get_hi() + splines[1].get_hi()))
        for i in range(1, n - 2):
            matrix_a[i][i - 1] = splines[i].get_hi() / (splines[i].get_hi() + splines[i + 1].get_hi())
            matrix_a[i][i] = 2
            matrix_a[i][i + 1] = splines[i + 1].get_hi() / (splines[i].get_hi() + splines[i + 1].get_hi())
        matrix_a[n - 2][n - 3] = splines[n - 2].get_hi() / (splines[n - 1].get_hi() + splines[n - 2].get_hi())
        matrix_a[n - 2][n - 2] = 2
    elif (matrix_a.rows == 1):
        matrix_a[0][0] = 2
    return matrix_a

def _create_matrix_b(sorted_points: list):
    n = len(sorted_points) - 1
    matrix_b = Matrix(rows=n-1, columns=1)
    for i in range(2, n + 1):
        matrix_b[i - 2][0] = 6 * _ui3(sorted_points, i)
    return matrix_b


def _ui2(sorted_points, index):
    p = sorted_points
    return (p[index]["y"] - p[index - 1]["y"]) / (p[index]["x"] - p[index - 1]["x"])

def _ui3(sorted_points, index):
    p = sorted_points
    return (_ui2(p, index) - _ui2(p, index - 1)) / (p[index]["x"] - p[index - 2]["x"])


def _get_ai(sorted_points: list, index: int):
    return sorted_points[index]["y"]

def _get_bi(sorted_points, index, c_arr):
    hi = sorted_points[index]["x"] - sorted_points[index - 1]["x"]
    ui = _ui2(sorted_points, index)
    return c_arr[index] * hi / 3 + c_arr[index - 1] / 6 * hi + ui

def _get_di(sorted_points, index, c_arr):
    hi = sorted_points[index]["x"] - sorted_points[index - 1]["x"]
    return (c_arr[index] - c_arr[index - 1]) / hi

