from keras.datasets import mnist
from sklearn.decomposition import PCA
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.multiclass import OneVsRestClassifier
import numpy
import pandas as pd

def main():
    (train_x, train_y), (test_x, test_y) = mnist.load_data()

    dim = 784
    train_x = train_x.reshape(len(train_x), dim)
    ev = 0.83
    M = 0
    pca = PCA(n_components=100, svd_solver='full')
    pca.fit(train_x, train_y)
    cum = numpy.cumsum(pca.explained_variance_ratio_)
    for i, e in enumerate(cum):
        if e > ev:
            M = i + 1
            break
    pca = PCA(n_components=M, svd_solver='full')
    pca.fit(train_x, train_y)
    data = pca.transform(train_x)
    train_x, test_x, train_y , test_y = train_test_split(data, train_y, test_size=0.3, random_state=126)
    print(sum(x[0] for x in train_x) / len(train_x))
    rfc = RandomForestClassifier(criterion='gini', min_samples_leaf=10, max_depth=20, n_estimators=10, random_state=126)
    ovrc = OneVsRestClassifier(rfc)
    ovrc.fit(train_x, train_y)
    data = ovrc.predict(test_x)
    cnt = 0
    for i in range(len(test_y)):
        if (test_y[i] == 5 and data[i] == 5):
            cnt += 1
    print(cnt)
    datafr = pd.read_csv("data/pred_for_task.csv", sep=",")
    data = datafr["file20"]
    print(data


if __name__ == "__main__":
    main()