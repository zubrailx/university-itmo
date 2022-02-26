import timeit
import sys
from lib import *


MATRIX_DIM_MIN = 1 
MATRIX_DIM_MAX = 20
MATRIX_GENERATE_MULTIPLICATION = 1000


def main():
    # read matrix dimensions
    try:
        matrix_dim = int(input("Enter matrix dimensions: "))
    except ValueError:
        print("ERROR: matrix dimension is NOT uint")
        return
    
    if not (MATRIX_DIM_MIN <= matrix_dim <= MATRIX_DIM_MAX):
        print("ERROR: matrix dimension is out of range")
        return

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
            return

    print("Enter matrix_b: ")
    matrix_b = matrix.matrix_read(matrix_dim, 1)
    matrix_x = Matrix().init(matrix_b.matrix)
    print("INFO: matrix_b == matrix_x")

    if (input("Do you want to diagonalize matrix? Y/N: ").strip().lower() == "y"):
        pair = matrix_a.create_diagonal_max_if_can()
        matrix_a = pair[0]
        temp_matrix_x = Matrix(matrix_x.rows, matrix_x.columns)
        temp_matrix_b = Matrix(matrix_b.rows, matrix_b.columns)
        lst_pairs = pair[1]
        for fr, to in lst_pairs:
            temp_matrix_b[to][0] = matrix_b[fr][0]
            temp_matrix_x[to][0] = matrix_x[fr][0]
        matrix_x = temp_matrix_x
        matrix_b = temp_matrix_b

    try:
        matrix_diff = float(input("Enter difference between iterations: "))
    except ValueError:
        print("ERROR: matrix difference is not float")
        return

    # start timer
    timer_start = timeit.default_timer()

    it_count = 0
    matrix_x_prev = matrix_x
    while (iteration.calculate_difference(matrix_x_prev, matrix_x) >= matrix_diff or it_count == 0):
        matrix_x_prev = matrix_x
        matrix_x = iteration.iterate(matrix_a, matrix_b, matrix_x)
        it_count += 1

    # stop timer
    elapsed_time = timeit.default_timer() - timer_start
    print("Result matrix_x: ", matrix_x, "Result matrix_err: ", matrix_x - matrix_x_prev, sep='\n')
    print("Number of iterations: ", it_count)
    print("Elapsed time (ms): ", elapsed_time * 1000)


if __name__ == "__main__":
    main()

