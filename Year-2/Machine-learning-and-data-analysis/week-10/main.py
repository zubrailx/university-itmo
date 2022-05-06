import cv2
import os
from sklearn.ensemble import BaggingClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.svm import LinearSVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import StackingClassifier
from sklearn.preprocessing import LabelEncoder


def extract_histogram(image, bins=(8, 8, 8)):
    hist = cv2.calcHist([image], [0, 1, 2], None, bins, [0, 256, 0, 256, 0, 256])
    cv2.normalize(hist, hist)
    return hist.flatten()

def main():
    trp = "data/train"
    tp = "data/test"
    image_train_paths = sorted([f for f in [os.path.join(trp, f) for f in os.listdir(trp)] if os.path.isfile(f)])
    image_test_paths = sorted([f for f in [os.path.join(tp, f) for f in os.listdir(tp)] if os.path.isfile(f)])
    train_data = []
    labels = []
    for (i, ip) in enumerate(image_train_paths):
        img = cv2.imread(ip, 1)
        label = ip.split(os.path.sep)[-1].split(".")[0]
        hist = extract_histogram(img)
        train_data.append(hist)
        labels.append(label)
    le = LabelEncoder()
    # cat - 0, dog - 1
    labels = le.fit_transform(labels)
    # mjakgiy zazor
    svm = LinearSVC(C=1.55, random_state=69)
    # trees + bagging
    dtc = DecisionTreeClassifier(criterion='entropy', min_samples_leaf=10, max_leaf_nodes=20, random_state=69)
    bc = BaggingClassifier(dtc, n_estimators=20, random_state=69)
    # forest
    rfc = RandomForestClassifier(n_estimators=20, criterion='entropy', min_samples_leaf=10,
                                 max_leaf_nodes=20, random_state=69)
    # 2.
    lr = LogisticRegression(solver='lbfgs', random_state=69)
    estimators = [('svm', svm), ('bc', bc), ('rfc', rfc)]
    sc = StackingClassifier(cv=2, final_estimator=lr, estimators=estimators)
    sc.fit(train_data, labels)
    # 3.
    print(sc.score(train_data, labels))
    # images
    single_image = cv2.imread('data/test/cat.1024.jpg')  # имя первой картинки
    histt = extract_histogram(single_image).reshape(1, -1)
    prediction = sc.predict_proba(histt)
    print(prediction)

    single_image = cv2.imread('data/test/cat.1003.jpg')  # имя первой картинки
    histt = extract_histogram(single_image).reshape(1, -1)
    prediction = sc.predict_proba(histt)
    print(prediction)

    single_image = cv2.imread('data/test/dog.1006.jpg')  # имя первой картинки
    histt = extract_histogram(single_image).reshape(1, -1)
    prediction = sc.predict_proba(histt)
    print(prediction)

    single_image = cv2.imread('data/test/dog.1022.jpg')  # имя первой картинки
    histt = extract_histogram(single_image).reshape(1, -1)
    prediction = sc.predict_proba(histt)
    print(prediction)



if __name__ == "__main__":
    main()