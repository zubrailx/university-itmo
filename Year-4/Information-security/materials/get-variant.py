#!/usr/bin/python3

if __name__ != "__main__":
    exit(0)

markina = [(1.1, 10), (1.2, 3), (2.1, 2), (2.2, 11), (2.3, 3)]
oziganov = [(1.1, 10), (1.21, 6), (1.22, 5), (2.1, 30), (2.5, 30), (2.6, 30)]

def get_variant(count, n):
    n = n - 1
    n = n % count
    n = n + 1
    return n


NUMBER = 12

print("Markina:")
for id, count in markina:
    n = get_variant(count, NUMBER)
    print(id, ":", n)


print("Oziganov:")
for id, count in oziganov:
    n = get_variant(count, NUMBER)
    print(id, ":", n)
