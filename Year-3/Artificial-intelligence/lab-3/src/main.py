import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split

dataset_path = "data/dataset.csv"

if __name__ != "__main__":
    print("It is not module")
    exit(-1)

data = pd.read_csv(dataset_path, delimiter=";")
data_x = data.drop("GRADE", axis=1)
data_y = data["GRADE"]
train_x , test_x, train_y, test_y = train_test_split(data_x, data_y, test_size=0.2, random_state=11)
print(data, sep="\n")
print(data_x, data_y, sep="\n")
print(train_x, train_y, test_x, test_y, sep="\n")
    
