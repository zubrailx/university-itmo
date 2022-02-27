from pandas import Series, DataFrame
import pandas as pd

data = pd.read_csv("data/data.csv", delimiter=',', decimal='.')
data = data[(data.region_name != "Томская область") & (data.region_name != "Самарская область")]
data = data.sort_values("salary")
data.index = list(range(1, len(data["salary"]) + 1))
print(data.loc[21]["salary"])
print(data.loc[37]["salary"])
print(data.loc[41]["salary"])
print(data["salary"].mean())
print(data["salary"].median())

