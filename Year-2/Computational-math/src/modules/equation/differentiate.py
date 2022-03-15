from copy import copy

from modules.parse import Node
from sympy import Symbol, diff


_PRECISION = 10 ** (-4)


def grad(f):
    return lambda x: (f(x + _PRECISION) - f(x)) / _PRECISION


def node_flatten(node: Node, var_dict, var_name):
    def f(x):
        var_dict_new = copy(var_dict)
        var_dict_new[var_name] = x
        return node.calculate(var_dict_new)
    return f


def sgrad(expr, x):
    return diff(expr, x)      # returns expression

def seval(expr, var_dict):
    return expr.evalf(subs=var_dict)

def sympy_flatten(node: Node, var_list):
    var_sim_list = []
    for i in range(len(var_list)):
        var_sim_list.append(Symbol(var_list[i]))
    var_sim_dict = dict()
    for i in range(len(var_list)):
        var_sim_dict[var_list[i]] = var_sim_list[i]
    return (node.calculate(var_sim_dict), var_sim_list)

