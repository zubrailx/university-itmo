import math

s = int(input("Enter first number: "))
e = int(input("Enter last number: "))
c = int(input("Enter task count: "))

print("Your tasks: ", end="")
for i in range(c):
    print(int(round(s + (e - s + 1) / c * i)), end=" ")
print()
