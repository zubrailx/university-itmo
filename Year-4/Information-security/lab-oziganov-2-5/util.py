def gcd_extended(a, b):
    if (a == 0):
        x = 0
        y = 1
        return (b, x, y)

    gcd, x, y = gcd_extended(b % a, a)
    x1 = x
    y1 = y

    x = y1 - (b // a) * x1
    y = x1

    return (gcd, x, y)


def mod_inverse(A, M):
    g, x, y = gcd_extended(A, M)
    if (g != 1):
        print("Inverse doesn't exist")
    else:
        res = (x % M + M) % M
        return res

print(gcd_extended(1000, 2000))
print(mod_inverse( 7423489, 65815655642676))
