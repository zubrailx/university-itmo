if __name__ != "__main__":
    print("It is not module")
    exit(-1)

import numpy as np
import pandas as pd
import math
import copy
import json
import operator
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split

DATASET_PATH = "/home/nikit/git/University-ITMO/Year-3/Artificial-intelligence/lab-3/data/dataset.csv"
TREE_JSON = "/home/nikit/git/University-ITMO/Year-3/Artificial-intelligence/lab-3/data/tree.json"
TREE_DEPTH = 6
VARIANT=11
Y_COLUMN = "GRADE"


# return list of unique attribures of X | Y
def calc_unique(df_xy: pd.DataFrame) -> dict:
    dct = {}
    for (name, data) in df_xy.items():
        arr = data.unique()
        arr.sort()
        dct[name] = arr
    return dct


def info_t(arr_ty: np.ndarray) ->float:
    result = 0.0
    tv, tc = np.unique(arr_ty, return_counts=True)
    t_norm = np.sum(tc)
    for j in range(len(tv)):
        result -= tc[j] / t_norm * math.log2(tc[j] / t_norm)
    return result


# разделить выборку по проверке x 
def df_split_by_att(arr_t: pd.DataFrame, x_name: str):
    result = []
    values = arr_t[x_name].unique()
    for e in values:
        result.append(arr_t[arr_t[x_name] == e])
    return result

def info_att_t(df_splitted: list) ->float:
    result = 0.0
    # columns - separated for this attribute values
    t_norm = sum([len(df) for df in df_splitted])
    for df in df_splitted:
        result += len(df) / t_norm * info_t(df[Y_COLUMN].to_numpy())
    return result


def split_info_att(df_splitted: list) -> float:
    t_norm = sum([len(df) for df in df_splitted])
    result = 0.0
    for df in df_splitted:
        result -= len(df) / t_norm * math.log2(len(df) / t_norm)
    return result
    

data = pd.read_csv(DATASET_PATH, delimiter=";", index_col="STUDENT ID")
# normalize data
data.loc[data[Y_COLUMN] < 3, Y_COLUMN] = 0
data.loc[data[Y_COLUMN] > 2, Y_COLUMN] = 1

data_x = data.drop(Y_COLUMN, axis=1)
data_y = data[Y_COLUMN]
lst = train_test_split(data_x, data_y, test_size=0.2, random_state=VARIANT)
train_x, test_x, train_y, test_y = pd.DataFrame(lst[0]), pd.DataFrame(lst[1]), pd.Series(lst[2]), pd.Series(lst[3])


dct_uniq = calc_unique(data)
# combine x and y
train = train_x.copy()
train[Y_COLUMN] = train_y
test = test_x.copy()
test[Y_COLUMN] = test_y


# build C4.5
def build_tree(tree_node: dict, df_subset: pd.DataFrame, level: int):
    if (level >= TREE_DEPTH):
        return

    tree_node["entropy"] = info_t(df_subset[Y_COLUMN].to_numpy())
    tree_node["samples"] = len(df_subset)
    
    dfx_columns = list(df_subset.columns)
    dfx_columns.remove(Y_COLUMN)
    gain_ratio = []
    for att in dfx_columns:
        df_splitted = df_split_by_att(df_subset, att)
        infoattx = info_att_t(df_splitted) 
        divt = tree_data["entropy"] - infoattx
        divr = split_info_att(df_splitted)
        if divr != 0.0:
            gain_ratio.append(divt / divr)
        else:
            gain_ratio.append(0)
    index_max = max(range(len(gain_ratio)), key=gain_ratio.__getitem__)
    tree_node["attr"] = dfx_columns[index_max]
    # split data by att values
    attr = tree_node["attr"]
    attr_values = dct_uniq[attr]
    df_subset_list = [df_subset[df_subset[attr] == val].drop(attr, axis=1) for val in attr_values]
    # if entropy == 0.0 then no more recursion required
    if tree_node["entropy"] == 0.0:
        tree_node["result"] = {}
        if tree_node["samples"] > 0:
            tree_node["result"][int(df_subset[Y_COLUMN].iloc[0])] = tree_node["samples"]
        return
    # another check on recursion
    if level + 1 < TREE_DEPTH:
        if tree_node["samples"] > 1:
            tree_node["children"] = []
            for dfs in df_subset_list:
                child_node = {}
                build_tree(child_node, dfs, level + 1)
                tree_node["children"].append(child_node)
    # if current depth is max then calculate entries 
    elif level + 1 == TREE_DEPTH:
        tree_node["result"] = df_subset[Y_COLUMN].value_counts().to_dict()
        


 
class Tree:
    def __init__(self, data: dict):
        self.root = data

    def predict_proba(self, row: pd.Series):
        res = self.predictp_helper(self.root, row)
        if res is None:
            return res
        else:
            s = sum(res.values())
            res = copy.deepcopy(res)
            for k, v in res.items():
                res[k] = v / s
            return res

    def predictp_helper(self, node, row: pd.Series):
        if "children" in node:
            attr = node["attr"]
            idx = int(np.where(dct_uniq[attr] == row[attr])[0])
            cnode = node["children"][idx]
            result = self.predictp_helper(cnode, row)
            # if children can't calculate value (because samples == 0) 
            # then try calculate on this level (sum all childs)
            if result is None or len(result.keys()) == 0:
                result = {}
                for child in node["children"]:
                    for k, v in child["result"].items():
                        result[k] = result.get(k, 0) + v
                return result
            else:
                return result
        else:
            if node["samples"] == 0:
                return None
            else:
                return node["result"]

    def predict(self, row: pd.Series):
        res = self.predictp_helper(self.root, row)
        if res is None:
            return None
        else:
            return max(res.items(), key=operator.itemgetter(1))[0]

def accuracy(tp, fp, fn, tn):
    if (tp + fp + fn + tn) == 0:
        return 0
    return (tp + tn) / (tp + fp + fn + tn)

def precision(tp, fp):
    if (tp + fp) == 0:
        return 0
    return tp / (tp + fp)

def recall(tp, fn):
    if (tp, fn) == 0:
        return 0
    return tp / (tp + fn)

def classificate(pred_y, true_y):
    tp = 0; fp = 0; fn = 0; tn = 0
    for i in range(min(len(pred_y), len(true_y))):
        tp += (pred_y[i] == 1 and true_y[i] == 1)
        fp += (pred_y[i] == 1 and true_y[i] == 0)
        tn += (pred_y[i] == 0 and true_y[i] == 0)
        fn += (pred_y[i] == 0 and true_y[i] == 1)
    return (tp, fp, fn, tn) 
                
# train algorithm
tree_data = {}
build_tree(tree_data, train, 0)
tree = Tree(tree_data)

# print(tree.predict_proba(data.iloc[0]))
# print(tree.predict(data.iloc[0]))

print(dct_uniq)
true_y = test_y.array
# print(true_y)
pred_y = []
for idx in range(len(test_x)):
    test_xi = test_x.iloc[idx]
    pred_y.append(tree.predict(test_xi))

tp, fp, fn, tn = classificate(pred_y, true_y)
v_acc = accuracy(tp, fp, fn, tn)
v_pre = precision(tp, fp)
v_rec = recall(tp, fn)
print("Accuracy: ", v_acc)
print("Precision: ", v_pre)
print("Recall: ", v_rec)

def draw_auc_roc():
    true_y = list(test_y.array)
    pred_y0 = []
    pred_y1 = []
    pred_y = []
    for idx in range(len(test_x)):
        test_xi = test_x.iloc[idx]
        prob = tree.predict_proba(test_xi)
        if prob is None:
            pred_y0.append(0)
            pred_y1.append(0)
        else:
            pred_y0.append(prob.get(0, 0))
            pred_y1.append(prob.get(1, 0))
        pred = tree.predict(test_xi)
        if pred is None:
            pred_y.append(-1)
        else:
            pred_y.append(pred)
    # print(true_y, pred_y0, pred_y1, pred_y, sep="\n")
    cords = [(0, 0)]
    fp = 0
    tp = 0
    for idx in range(len(pred_y)):
        pr1 = pred_y[idx]
        if pr1 != 1:
            continue
        if pr1 == true_y[idx]:
            tp += 1
        else:
            fp += 1
        cords.append((fp, tp))

    fp, tp = map(list, zip(*cords))

    pos_count = tp[-1] if len(tp) else 0
    neg_count = fp[-1] if len(fp) else 0

    tp = [tpi / pos_count for tpi in tp]
    fp = [fpi / neg_count for fpi in fp]
    # print(fp, tp)
    # print(pos_count, neg_count, sep="\n")

    plt.figure()
    lw = 2
    plt.plot(
        fp,
        tp,
        color="darkorange",
        lw=lw
    )
    plt.plot([0, 1], [0, 1], color="navy", lw=lw, linestyle="--")
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.0])
    plt.xlabel("False Positive Rate")
    plt.ylabel("True Positive Rate")
    plt.title("AUC ROC")
    plt.legend(loc="lower right")
    plt.savefig("auc-roc.png")
    plt.clf()


def draw_auc_pr():
    true_y = list(test_y.array)
    pred_y0 = []
    pred_y1 = []
    for idx in range(len(test_x)):
        test_xi = test_x.iloc[idx]
        prob = tree.predict_proba(test_xi)
        if prob is None:
            pred_y0.append(0)
            pred_y1.append(0)
        else:
            pred_y0.append(prob.get(0, 0))
            pred_y1.append(prob.get(1, 0))

    pre_scores = []
    rec_scores = []
    # probability thresholds
    prob_thr = np.linspace(0, 1, num=20)
    for p in prob_thr:
        pred_y = []
        for prob in pred_y1:
            if prob > p:
                pred_y.append(1)
            else:
                pred_y.append(0)
        tp, fp, fn, tn = classificate(pred_y, true_y)
        pre_scores.append(precision(tp, fp))
        rec_scores.append(recall(tp, fn))

    plt.figure()
    lw = 2
    plt.plot(
        rec_scores,
        pre_scores,
        color="darkorange",
        lw=lw
    )
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.0])
    plt.xlabel("Recall")
    plt.ylabel("Precision")
    plt.legend(loc="lower right")
    plt.title("AUC PR")
    plt.savefig("auc-pr.png")
    plt.clf()

draw_auc_roc()
draw_auc_pr()

# print tree
class NpEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, np.integer):
            return int(obj)
        if isinstance(obj, np.floating):
            return float(obj)
        if isinstance(obj, np.ndarray):
            return obj.tolist()
        return super(NpEncoder, self).default(obj)

with open(TREE_JSON, 'w') as f:
    json.dump(tree_data,  f, cls=NpEncoder, indent=2)
# print("INFO_X_T:", info_x_t())

# print(data, sep="\n")
# print(data_x, data_y, sep="\n")
# print(train_x, train_y, test_x, test_y, sep="\n")
