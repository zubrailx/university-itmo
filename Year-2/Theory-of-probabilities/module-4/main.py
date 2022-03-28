import math 

def c_n_k(n: int, k: int) :
    return math.factorial(n)/math.factorial(n - k) / math.factorial(k)

def main():
    print(c_n_k(10, 4) * (15/216)**4 * (201/216)**6)

if __name__ == "__main__":
    main()
