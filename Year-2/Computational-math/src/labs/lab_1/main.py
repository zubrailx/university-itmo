import timeit
import sys

import modules.matrix as matrix
from modules.matrix import Matrix
from modules.util import ProjectException

from . import iteration

MATRIX_DIM_MIN = 1 
MATRIX_DIM_MAX = 20
MATRIX_GENERATE_MULTIPLICATION = 1000

def solve(ifs, ofs):
    data = _read_stdin() if ifs == sys.stdin else _read_ifile()
    # start timer
    timer_start = timeit.default_timer()
    if (data is not None):
        matrix_a, matrix_b, matrix_x, matrix_diff = data[0], data[1], data[2], data[3]
        try:
            mx, it_cnt, err = iteration.solve_iterate(matrix_a, matrix_b, matrix_x, matrix_diff)
            # stop timer
            elapsed_time = timeit.default_timer() - timer_start
            print("Elapsed time (ms): ", elapsed_time * 1000)

            ofs.write(mx + "\n")
            ofs.write("Number of iterations: " + str(it_cnt) + "\n")
            ofs.write("Error:" + str(err))
        except ProjectException as e:
            print(e)


def _read_stdin():
    # read matrix dimensions
    try:
        matrix_dim = int(input("Enter matrix dimensions: "))
    except ValueError:
        print("ERROR: matrix dimension is NOT uint")
        return None

    if not (MATRIX_DIM_MIN <= matrix_dim <= MATRIX_DIM_MAX):
        print("ERROR: matrix dimension is out of range")
        return None

    if (input("Generate matrix? Y/N: ").strip().lower() == "y"):
        if (input("Generate pseudo? Y/N: ").strip().lower() == "y"):
            matrix_a = matrix.matrix_generate_pseudo_square(matrix_dim, matrix_dim, MATRIX_GENERATE_MULTIPLICATION)
        else:
            matrix_a = matrix.matrix_generate(matrix_dim, matrix_dim, MATRIX_GENERATE_MULTIPLICATION)
            while (not matrix_a.is_convergent()):
                matrix_a = matrix.matrix_generate(matrix_dim, matrix_dim, MATRIX_GENERATE_MULTIPLICATION)
        print("Generated matrix:", matrix_a, sep='\n')
    else:
        matrix_a = matrix.matrix_read(matrix_dim, matrix_dim)
        if (not matrix_a.is_convergent()):
            print("CHECK: matrix is not convergent")
            print("Your matrix:", matrix_a, sep='\n')
            return None

    print("Enter matrix_b: ")
    matrix_b = matrix.matrix_read(matrix_dim, 1)
    if (input("Do you want to diagonalize matrix? Y/N: ").strip().lower() == "y"):
        matrix_a = matrix_a.create_diagonal_max_if_can(matrix_b)[0]
    print("INFO: matrix_b == matrix_x")
    matrix_x = _generate_matrix_x(matrix_b=matrix_b)
    try:
        matrix_diff = float(input("Enter difference between iterations: "))
    except ValueError:
        print("ERROR: matrix difference is not float")
        return None
    return (matrix_a, matrix_b, matrix_x, matrix_diff)


def _generate_matrix_x(dim=None, matrix_b=None):
    if (matrix_b is None and dim is not None):
        if (dim is not None):
            return Matrix(dim, 1).init(0)
        else:
            raise ValueError("Invalid arguments")
    else:
        return Matrix().init(matrix_b.matrix)


def _read_ifile():
    return None
