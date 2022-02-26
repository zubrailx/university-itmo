import random
from copy import deepcopy, copy

from . import util


class Matrix:
    def __init__(self, rows=0, columns=0):
        if (isinstance(rows, int) and (isinstance(columns, int))):
            self.matrix = []
            self.rows = rows
            self.columns = columns
            for _ in range(rows):
                self.matrix.append([None for _ in range(columns)])
        else:
            raise Exception("ERROR: Matrix type check")

    def init(self, m):
        if (isinstance(m, float) or isinstance(m, int)):
            self.matrix = [[m] * self.columns for _ in range(self.rows)]
        elif (isinstance(m, list) and (isinstance(m[0], list))):
            # copy deep the matrix
            self.matrix = deepcopy(m)
            self.rows = len(m)
            self.columns = len(m[0])
        else:
            raise Exception("ERROR: Matrix type check")
        return self

    def __str__(self):
        s = "[" 
        for i in range(self.rows - 1):
            s += str(self.matrix[i]) + '\n'
        s += str(self.matrix[self.rows - 1]) + ']'
        return s

    def __getitem__(self, row):
        return self.matrix[row]

    def __setitem__(self, row):
        return self.matrix[row]

    def __add__(self, arg):
        matrix = Matrix(self.rows, self.columns).init(0)
        if (isinstance(arg, Matrix)):
            if (self.rows == arg.rows and self.columns == arg.columns):
                for i in range(self.rows):
                    for j in range(self.columns):
                        matrix.matrix[i][j] = self.matrix[i][j] + arg.matrix[i][j]
            else:
                raise Exception("ERROR: matrix dimension check")
        else:
            raise Exception("ERROR: Matrix type check")
        return matrix

    def __sub__(self, arg):
        matrix = Matrix(self.rows, self.columns).init(0)
        if (isinstance(arg, Matrix)):
            if (self.rows == arg.rows and self.columns == arg.columns):
                for i in range(self.rows):
                    for j in range(self.columns):
                        matrix.matrix[i][j] = self.matrix[i][j] - arg.matrix[i][j]
            else:
                raise Exception("ERROR: matrix dimension check")
        else:
            raise Exception("ERROR: Matrix type check")
        return matrix

    def __mul__(self, arg):
        return _matrix_mul(self, arg)

    def __rmul__(self, arg):
        return _matrix_mul(self, arg)

    def __truediv__(self, arg):
        matrix = Matrix().init(self.matrix)
        if (isinstance(arg, int) or isinstance(arg, float)):
            for i in range(self.rows):
                for j in range(self.columns):
                    matrix[i][j] /= arg
        else:
            raise Exception("ERROR: matrix type check")
        return matrix

    def insert_row(self, row, i):
        if (isinstance(row, list) and isinstance(i, int)):
            if (self.rows > i):
                self.matrix[i] = copy(row)
            else:
                raise Exception("ERROR: index out of bounds")
        else:
            raise Exception("ERROR: matrix type check")

    # doesn't return new matrix (OPTIMAL)
    def swap_rows(self, before, after):
        if (isinstance(before, int) and isinstance(after, int)):
            temp = self.matrix[before]
            self.matrix[before] = self.matrix[after]
            self.matrix[after] = temp
        else:
            raise Exception("ERROR: matrix type check")

    # doesn't return new matrix (OPTIMAL)
    def swap_columns(self, before, after):
        if (isinstance(before, int) and isinstance(after, int)):
            for i in range(self.columns):
                temp = self.matrix[before][i]
                self.matrix[before][i] = self.matrix[after][i]
                self.matrix[after][i] = temp
        else:
            raise Exception("ERROR: matrix type check")

    def is_convergent(self):
        return _matrix_convergent_cons_2(self)

    def create_diagonal_max_if_can(self):
        if (self.rows != self.columns or self.rows == 0):
            raise Exception("ERROR: bad matrix")
        # check that maximum elements
        # NOTE if there are more than one max elements in one row/column - undefined permutations
        perm = []   # (from, to)
        lst_dest = []
        for i in range(self.rows):
            max_e = self.matrix[i][0]
            index = 0
            for j in range(1, self.columns):
                if (max_e < self.matrix[i][j]):
                    max_e = self.matrix[i][j]
                    index = j
            if (index in lst_dest):
                return (Matrix().init(self.matrix), perm)
            else:
                perm.append((i, index))
                lst_dest.append(index)
        matrix_new = Matrix(self.rows, self.columns)
        for i in range(len(perm)):  # len(perm) always eq to self.rows
            matrix_new.insert_row(self.matrix[perm[i][0]], perm[i][1])
        return (matrix_new, perm)

# << END class Matrix


def _matrix_convergent_cons_2(matrix):
    if (matrix.rows != matrix.columns):
        return False
    matrix = matrix.create_diagonal_max_if_can()[0]
    return _matrix_convergent_cons_2_a(matrix) or _matrix_convergent_cons_2_b(matrix)


def _matrix_convergent_cons_2_a(matrix):
    is_convergent = True
    for i in range(matrix.rows):
        main_element = 2 * abs(matrix[i][i])
        # check on one row
        for j in range(matrix.columns):
            main_element -= abs(matrix[i][j])
        if main_element <= 0:
            is_convergent = False
            break
    return is_convergent


def _matrix_convergent_cons_2_b(matrix):
    is_convergent = True
    for i in range(matrix.columns):
        main_element = 2 * abs(matrix[i][i])
        # check on one column
        for j in range(matrix.rows):
            main_element -= abs(matrix[j][i])
        if main_element <= 0:
            is_convergent = False
            break
    return is_convergent


def matrix_read(rows, columns):
    # init matrix with 0
    matrix = Matrix(rows, columns).init(0)
    print("Enter the matrix by rows:")
    i = 0
    while i < rows:
        s = input(f"[{i + 1}]: ").split()
        for j in range(columns):
            e = s[j]
            if not util.is_number(e):
                accepted = False
                while not accepted:
                    e = input(f"Element [{i + 1}][{j + 1}] = {e} is invalid, retry: ")
                    accepted = util.is_number(e)
            matrix[i][j] = float(e)
        i += 1
    return matrix


def matrix_generate(rows, columns, mul):
    matrix = Matrix(rows, columns)
    for i in range(rows):
        for j in range(columns):
            matrix[i][j] = random.random() * mul
    return matrix

def matrix_generate_pseudo_square(rows, columns, mul):
    coef = 2
    matrix = matrix_generate(rows, columns, mul)
    while (not matrix.is_convergent()):
        for i in range(matrix.rows):
            matrix[i][i] *= coef
        matrix = matrix / coef
    return matrix


def _matrix_mul(arg1, arg2):
    if (isinstance(arg2, int) or isinstance(arg2, float)):
        # copy matrix
        matrix = Matrix().init(arg1.matrix)
        for i in range(arg1.rows):
            for j in range(arg1.columns):
                matrix[i][j] *= arg2
    elif (isinstance(arg2, Matrix)):
        if (arg1.columns == arg2.rows):
            # init matrix with zeroes
            matrix = Matrix(arg1.rows, arg2.columns).init(0)
            for i in range(matrix.rows):
                for j in range(matrix.columns):
                    # multiplication
                    for k in range(arg1.columns):
                        matrix[i][j] += arg1.matrix[i][k] * arg2.matrix[k][j]
        else:
            raise Exception("ERROR: matrix dimension check")
    else:
        raise Exception("ERROR: matrix type check")
    return matrix

