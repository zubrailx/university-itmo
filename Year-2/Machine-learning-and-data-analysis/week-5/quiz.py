def dist_evkl(p1, p2):
    return sum([(p1[i] - p2[i]) ** 2 for i in range(len(p1))]) ** 0.5

def dist_manh(p1, p2):
    return sum([abs(p1[i] - p2[i])  for i in range(len(p1))])

def dist_max(p1, p2):
    return max([abs(p1[i] - p2[i])  for i in range(len(p1))])


def calculate_classes(data, p, norm, k):
    dist = []
    mcl = {}
    for i in range(len(data)):
        cord_e = [data[i][0], data[i][1]]
        dist.append((norm(cord_e, p), i))
    dist.sort() 
    for i in range(k):
        mcl[data[dist[i][1]][2]] = mcl.get(data[dist[i][1]][2], 0) + 1
    return (sorted(mcl.items(), key=lambda x: x[1], reverse=True), dist)


def calculate_weight(data, p, weigh, k):
    dist = []
    mcl = {}
    for i in range(len(data)):
        cord_e = [data[i][0], data[i][1]]
        dist.append((weigh(cord_e, p), i))
    for i in range(k):
        mcl[data[dist[i][1]][2]] = mcl.get(data[dist[i][1]][2], 0) + dist[i][0]
    return (sorted(mcl.items(), key=lambda x: x[1], reverse=True), dist)

def main():
    print("QUIZ-1")
    point_x = (1, 3, -5, 7)
    point_x_sh = (2, -4, 0, 8)
    print("evkl: ", dist_evkl(point_x, point_x_sh))
    print("manh: ", dist_manh(point_x, point_x_sh))
    print("max: ", dist_max(point_x, point_x_sh))

    print("QUIZ-2")
    data = ((28, 10, 1), (49, 49, 1), (48, 35, 0), (36, 33, 1), (45, 54, 0))
    point_x = (33, 47)
    print("class: ", calculate_classes(data, point_x, dist_evkl, 1))
    print("class: ", calculate_classes(data, point_x, dist_manh, 1))
    print("class: ", calculate_classes(data, point_x, dist_max, 1))

    print("QUIZ-3")
    print("weight: ", calculate_weight(data, point_x, lambda p1, p2: 1/dist_evkl(p1, p2) ** 2, 5))
    return


if __name__ == "__main__":
    main()
