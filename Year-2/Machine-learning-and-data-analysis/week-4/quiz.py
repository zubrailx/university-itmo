def mean(arr):
    return sum(arr)/len(arr)

def lineur_regression(x_arr, y_arr):
    if (len(x_arr) != len(y_arr)):
        raise Exception("ERROR: length not equals")
    x_mean = mean(x_arr)
    y_mean = mean(y_arr)
    num_sum, den_sum = 0, 0
    for i in range(len(x_arr)):
        num_sum += (x_arr[i] - x_mean) * (y_arr[i] - y_mean)
        den_sum += (x_arr[i] - x_mean) ** 2
    theta_1 = num_sum / den_sum
    theta_0 = y_mean - theta_1 * x_mean
    return (theta_0, theta_1)


def main():
    x_arr = [13, 4, 11, 20]
    y_arr = [8, 5, 6, 15]
    x_mean = mean(x_arr)
    y_mean = mean(y_arr)
    print(x_mean)
    print(y_mean)
    print(lineur_regression(x_arr, y_arr))


if __name__ == "__main__":
    main()
