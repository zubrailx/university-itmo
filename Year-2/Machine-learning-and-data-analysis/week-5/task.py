from sklearn.neighbors import KNeighborsClassifier


def dist_evkl(p1, p2):
    return sum([(p1[i] - p2[i]) ** 2 for i in range(len(p1))]) ** 0.5

def dist_manh(p1, p2):
    return sum([abs(p1[i] - p2[i])  for i in range(len(p1))])

def dist_max(p1, p2):
    return max([abs(p1[i] - p2[i])  for i in range(len(p1))])


def calculate_knn(data, p, norm, k):
    # elements(id(ignored), x, y, class)
    dist = []
    mcl = {}
    for i in range(len(data)):
        cord_e = [data[i][1], data[i][2]]
        dist.append((norm(cord_e, p), data[i]))
    dist.sort() 
    for i in range(min(k, len(dist))):
        mv = mcl.get(dist[i][1][3], {"count": 0, "ids" : [], "weight": 0})
        mv["count"] += 1
        mv["ids"].append(dist[i][1][0])
        mv["weight"] += dist[i][0]
        mcl[dist[i][1][3]] = mv
    return (sorted(mcl.items(), key=lambda x: x[1]["count"], reverse=True), dist)


def main():
    task_1()
    task_2()
    return


def task_1():
    data = []
    file = open("data/task-1.csv", "r")
    line = file.readline()
    while line:
        data.append(tuple(int(e) for e in line.split(',')))
        line = file.readline()
    point_x = (52, 87)

    print("TASK-1. Using own library.")
    print("Nearest neighbour: ", calculate_knn(data, point_x, dist_evkl, 1)[0])
    print()
    print("Nearest neighbours ids: ", calculate_knn(data, point_x, dist_evkl, 3)[1][:3])
    print()
    print("Find class: ", calculate_knn(data, point_x, dist_evkl, 3)[0])
    print()
    print("Nearest neighbour: ", calculate_knn(data, point_x, dist_manh, 1)[0])
    print()
    print("Nearest neighbours ids: ", calculate_knn(data, point_x, dist_manh, 3)[1][:3])
    print()
    print("Find class: ", calculate_knn(data, point_x, dist_manh, 3)[0])
    print()
    return


def task_2():
    string = "Access Cash Remove Refund Million Investment Bonus"
    print("done")
    return


if __name__ == "__main__":
    main()
