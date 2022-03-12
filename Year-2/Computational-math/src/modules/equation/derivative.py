from copy import copy

from modules.parse.node import Node


_PRECISION = 0.0001


def grad(f):
    return lambda x: (f(x + _PRECISION) - f(x)) / _PRECISION


def node_flatten(node: Node, var_dict, var_name):
    def f(x):
        var_dict_new = copy(var_dict)
        var_dict_new[var_name] = x
        return node.calculate(var_dict_new)
    return f
