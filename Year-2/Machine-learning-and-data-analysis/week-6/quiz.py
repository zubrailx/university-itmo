import math

def main():
    # quiz-2
    arr_theta = (-0.046, 0.541, -0.014, -0.132)
    arr_x = (0, 4, 40, 8)
    print(1/(1 + math.e ** -(arr_theta[0] + sum([arr_theta[i] * arr_x[i] for i in range(1, len(arr_x))]))))
    return


if __name__ == "__main__":
    main()
