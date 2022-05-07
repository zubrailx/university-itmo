from sympy import linsolve, symbols
from sympy.parsing.sympy_parser import parse_expr

def main():
    v1s, v2s, v3s, v4s = symbols("v1, v2, v3, v4")

    v1 = "-v1 + 0.4 * (0.3 * (2.0 + 0.8 * v2) + 0.7 * (3.0 + 0.8 * v3)) + 0.6 * (1 * (1.0 + 0.8 * v3))"
    v2 = "-v2 + 1.0 * (1.0 * (3.0 + 0.8 * v1))"
    v3 = "-v3 + 0.5 * (1.0 * (-3.0 + 0.8 * v1)) + 0.5 * (0.2 * (1.0 + 0.8 * v3) + 0.8 * (6.0 + 0.8 * v4))"
    v4 = "-v4 + 1.0 * (0.6 * (5.0 + 0.8 * v1) + 0.4 * (-3.0 + 0.8 * v2))"
    v1_expr = parse_expr(v1)
    v2_expr = parse_expr(v2)
    v3_expr = parse_expr(v3)
    v4_expr = parse_expr(v4)
    eqns = [v1_expr, v2_expr, v3_expr, v4_expr]
    print([round(x, 3) for x in list(linsolve(eqns, v1s, v2s, v3s, v4s))[0]])


if __name__ == "__main__":
    main()
