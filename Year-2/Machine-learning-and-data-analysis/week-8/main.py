import os
from pandas import DataFrame
import pandas as pd
from sklearn.tree import DecisionTreeClassifier
from sklearn.tree import export_graphviz
import graphviz
from sklearn.metrics import accuracy_score
from sklearn.metrics import f1_score


def path():
    pass


def main():
    datall = pd.read_csv("data/diabetes.csv")
    data = datall.head(660)
    print(len(data[data["Outcome"] == 0]))
    coef = 0.8
    train = data.head(int(len(data) * coef))
    test = data.tail(int(len(data) * (1 - coef)))
    x_train = train.drop('Outcome', axis=1)
    y_train = DataFrame(train["Outcome"])
    dtc = DecisionTreeClassifier(criterion='entropy', max_leaf_nodes=5, min_samples_leaf=5, random_state=2020)
    dtc.fit(x_train, y_train)
    print(dtc.tree_.max_depth)

    columns = list(x_train.columns)
    export_graphviz(dtc, out_file='data/tree.dot',
                    feature_names=columns,
                    class_names=['0', '1'],
                    rounded=True, proportion=False,
                    precision=2, filled=True, label='all')

    with open('data/tree.dot') as f:
        dot_graph = f.read()
    graph = graphviz.Source(dot_graph)
    graph.render(view=True)

    x_test = test.drop('Outcome', axis=1)
    y_test = DataFrame(test["Outcome"])
    y_pred = dtc.predict(x_test)
    print(accuracy_score(y_test, y_pred))
    print(f1_score(y_test, y_pred, average='macro'))

    indexes = [751, 748, 754, 746]
    for i in indexes:
        line = datall.loc[i].to_frame().T.drop('Outcome', axis=1)
        print(dtc.predict(line))

    # delete file
    if (os.path.exists('Source.gv')):
        os.remove('Source.gv')


if __name__ == "__main__":
    path()
    main()
