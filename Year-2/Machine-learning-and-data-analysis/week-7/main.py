import os
from sklearn.svm import LinearSVC
from sklearn.metrics import classification_report
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import train_test_split
from sklearn.metrics import f1_score
import numpy as np
import cv2

def extract_histogram(image, bins=(8, 8, 8)):
    hist = cv2.calcHist([image], [0, 1, 2], None, bins, [0, 256, 0, 256, 0, 256])
    cv2.normalize(hist, hist)
    return hist.flatten()

def main():
    trp = "data/train"
    tp = "data/test"
    image_train_paths = sorted([f for f in [os.path.join(trp, f) for f in os.listdir(trp)] if os.path.isfile(f)])
    image_test_paths = sorted([f for f in [os.path.join(tp, f) for f in os.listdir(tp)] if os.path.isfile(f)])
    data = []
    labels = []
    for (i, imagePath) in enumerate(image_train_paths):
        image = cv2.imread(imagePath, 1)
        label = imagePath.split(os.path.sep)[-1].split(".")[0]
        hist = extract_histogram(image)
        data.append(hist)
        labels.append(label)
    le = LabelEncoder()
    labels = le.fit_transform(labels)
    (train_data, test_data, train_labels, test_labels) = train_test_split(np.array(data), labels, test_size=0.25, random_state=12)  # random_state поменять на свой
    model = LinearSVC(random_state=12, C=0.65)  # random_state и C поменять на свои
    model.fit(train_data, train_labels)
    predictions = model.predict(test_data)
    print(round(model.coef_[0][385] ,2))
    print(round(model.coef_[0][475], 2))
    print(round(model.coef_[0][167], 2))
    # for dog == 1
    tp = sum([test_labels[i] == predictions[i] and predictions[i] == 1 for i in range(len(predictions))]) / len(predictions)
    tn = sum([test_labels[i] == predictions[i] and predictions[i] == 0 for i in range(len(predictions))]) / len(predictions)
    fp = sum([test_labels[i] != predictions[i] and predictions[i] == 1 for i in range(len(predictions))]) / len(predictions)
    fn = sum([test_labels[i] != predictions[i] and predictions[i] == 0 for i in range(len(predictions))]) / len(predictions)
    pr = tp / (tp + fp)
    re = tp / (tp + fn)
    dog_f1 = 2 * (pr * re) / (pr + re)
    # for cat
    tp = sum([test_labels[i] == predictions[i] and predictions[i] == 0 for i in range(len(predictions))]) / len(predictions)
    tn = sum([test_labels[i] == predictions[i] and predictions[i] == 1 for i in range(len(predictions))]) / len(predictions)
    fp = sum([test_labels[i] != predictions[i] and predictions[i] == 0 for i in range(len(predictions))]) / len(predictions)
    fn = sum([test_labels[i] != predictions[i] and predictions[i] == 1 for i in range(len(predictions))]) / len(predictions)
    # for cat == 1
    pr = tp / (tp + fp)
    re = tp / (tp + fn)
    cat_f1 = 2 * (pr * re) / (pr + re)
    # print(classification_report(test_labels, predictions, target_names=le.classes_))
    # print(round(f1_score(test_labels, predictions, average='macro'), 2))
    print(round((dog_f1 + cat_f1) / 2, 2))

    single_image = cv2.imread('data/test/cat.1017.jpg')  # имя первой картинки
    histt = extract_histogram(single_image).reshape(1, -1)
    prediction = model.predict(histt)
    print(prediction)

    single_image = cv2.imread('data/test/cat.1041.jpg')  # имя первой картинки
    histt = extract_histogram(single_image).reshape(1, -1)
    prediction = model.predict(histt)
    print(prediction)

    single_image = cv2.imread('data/test/cat.1014.jpg')  # имя первой картинки
    histt = extract_histogram(single_image).reshape(1, -1)
    prediction = model.predict(histt)
    print(prediction)

    single_image = cv2.imread('data/test/cat.1042.jpg')  # имя первой картинки
    histt = extract_histogram(single_image).reshape(1, -1)
    prediction = model.predict(histt)
    print(prediction)


if __name__ == "__main__":
    main()


















