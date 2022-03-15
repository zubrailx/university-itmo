from modules.parse import tokenize, parse
from modules.equation import node_flatten, grad
from modules.parse import Node, parse_expression
from modules.equation.differentiate import sympy_flatten


while True:
    expression = input("ENTER: ")
    expression, var_lst = parse.parse_expression(expression)
    se, sv = sympy_flatten(expression, var_lst)
    print(se, sv)