import pandas as pd
import numpy as np
from sklearn.cluster import KMeans

def main():
    data = pd.read_csv("data/input.csv", delimiter=',', index_col='Object')
    km = KMeans(n_clusters=3, init=np.array([[8.5, 8.75], [10.5, 7.13], [12.0, 5.0]]), max_iter=100, n_init=1)
    x_data = data.drop("Cluster", axis=1)
    km.fit(x_data)
    labels = np.array(km.labels_)
    distance = np.array(km.fit_transform(x_data))
    print(labels)
    # centroid0 = km.cluster_centers_[0]
    # print(centroid0)
    dataframe = pd.DataFrame(distance)
    dataframe = dataframe.assign(Cluster=labels)
    dataframe = pd.DataFrame(dataframe[(dataframe.Cluster == 0)])
    print(dataframe.mean().loc[0])


if __name__ == "__main__":
    main()