import math 

def c_n_k(n: int, k: int) :
    return math.factorial(n)/math.factorial(n - k) / math.factorial(k)

def main():
    p_a  = sum([c_n_k(10, i) * (5/6)**(10 - i) * (1/6) ** i for i in range(1, 11)])
    p_ab = sum([c_n_k(10, i) * (5/6)**(10 - i) * (1/6) ** i for i in range(2, 11)])

    print(p_ab/p_a)

if __name__ == "__main__":
    main()
