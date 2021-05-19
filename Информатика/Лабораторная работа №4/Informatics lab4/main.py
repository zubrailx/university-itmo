import random
file = open("try_one_10", encoding='utf-8')
string = file.readline()
a = []

while string != "":
    a.append(string.replace("\n",""))
    string = file.readline()

for i in range(len(a)):
    ran = random.randint(0, len(a) - 1)
    t = a[i]
    a[i] = a[ran]
    a[ran] = t

for i in range(len(a)):
    print(a[i])