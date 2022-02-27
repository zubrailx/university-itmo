from pandas import Series, DataFrame
import pandas as pd
from sklearn.linear_model import LinearRegression

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

    # caclulate R
    num_sum, den_sum = 0, 0
    for i in range(len(x_arr)):
        num_sum += (y_arr[i] - theta_0 - theta_1 * x_arr[i]) ** 2
        den_sum += (y_arr[i] - y_mean) ** 2
    r_pow_2 = 1 - num_sum / den_sum
    return (theta_0, theta_1, r_pow_2)


def main():
    # task-1
    x_arr = [6, 25, 4, 18, 3, 7, 17, 12, 13, 5]
    y_arr = [12, 52, 16, 35, 6, 16, 32, 21, 22, 8]
    x_mean = mean(x_arr)
    y_mean = mean(y_arr)
    print("Выборочное среднее X: ", x_mean)
    print("Выборочное среднее Y: ", y_mean)
    print(lineur_regression(x_arr, y_arr))

    # task-2
    data = pd.DataFrame(pd.read_csv("data/candy-data.csv", index_col="competitorname"))
    data_caramel = data.loc["Caramel Apple Pops"].to_frame().T.drop(["winpercent", "Y"], axis=1)
    data_hersheys = data.loc["Hersheys Kisses"].to_frame().T.drop(["winpercent", "Y"], axis=1)
    data = pd.DataFrame(data.drop(["Caramel Apple Pops", "Hersheys Kisses"]))
    print("DATA: ", data, sep='\n')
    data_x = pd.DataFrame(data.drop(["winpercent", "Y"], axis=1))
    print("X: ", data_x, sep='\n')
    data_y = data["winpercent"]
    print("Y: ", data_y, sep='\n')
    reg = LinearRegression().fit(data_x, data_y)
    print("PREDICTIONS")
    print(reg.predict(data_caramel))
    print(reg.predict(data_hersheys))
    print(reg.predict([[1, 0, 0, 1, 1, 0, 0, 1, 1, 0.624, 0.438]]))

    print("COEFFICIENTS")
    print(reg.intercept_, *reg.coef_)


if __name__ == "__main__":
    main()
