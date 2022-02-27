from sklearn.linear_model import LogisticRegression
from pandas import Series, DataFrame
import pandas as pd
import numpy as np
from sklearn import metrics

def main():
    task_1()
    return


def task_1():
    # Train Data
    data_learn = DataFrame(pd.read_csv("data/candy-data.csv", index_col="competitorname"))
    data_learn = DataFrame(data_learn.drop(["Mike & Ike", "Root Beer Barrels", "Starburst"]))
    data_learn_x = data_learn.drop(["winpercent", "Y"], axis = 1)
    data_learn_y = data_learn["Y"]
    # Test Data
    data_test = DataFrame(pd.read_csv("data/candy-test.csv", index_col="competitorname"))
    data_test_x = DataFrame(data_test.drop(["Y"], axis = 1))
    data_test_wer = data_test_x.loc["Werthers Original Caramel"]
    data_test_sup = data_test_x.loc["Super Bubble"]
    # Testing
    log_reg = LogisticRegression(random_state = 2019, solver = "lbfgs").fit(data_learn_x, data_learn_y)
    print("RESULT for [Werthers, Super]")
    print(log_reg.predict_proba([data_test_wer, data_test_sup]))
    print("OTHER RESULTS")
    y_true = data_test["Y"].values
    y_pred = log_reg.predict(data_test_x)
    fp = np.sum((y_pred == 1) & (y_true == 0))
    tp = np.sum((y_pred == 1) & (y_true == 1))
    fn = np.sum((y_pred == 0) & (y_true == 1))
    tn = np.sum((y_pred == 0) & (y_true == 0))
    print(y_pred)
    print(y_true)
    print("TPR: ", tp / (tp + fn))
    print("Presicion: ", tp / (tp + fp)) 
    print("AUC: ", metrics.roc_auc_score(y_true, y_pred))
    return


if __name__ == "__main__":
    main()
