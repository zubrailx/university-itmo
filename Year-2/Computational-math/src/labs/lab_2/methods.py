import math
from modules import equation

# argument data:
#   method  
#   equation  
#   data       : range_min, range_max, x_0, iterations
def split_half(data: dict) -> dict:
    node_root, var_list = equation.parse_expression(data["equation"])
    assert(len(var_list) == 1)
    range_min = data["data"]["range_min"]
    range_max = data["data"]["range_max"]
    fval_range_min = node_root.calculate({var_list[0] : range_min})
    fval_range_max = node_root.calculate({var_list[0] : range_max})
    if (sum([fval_range_min > 0, fval_range_max > 0]) != 1):
        return {"error": "Invalid arguments. Function values of borders are same sigh."}
    for _ in range(data["data"]["iteration"]):
        range_med = (range_max + range_min) / 2
        fval_range_med = node_root.calculate({var_list[0] : range_med})
        if (sum([fval_range_med > 0, fval_range_min > 0]) == 1):
            range_max = range_med
        else:
            range_min = range_med
    return {"result": { "range_min" : range_min, "range_max" : range_max }}


def tangent(data:dict)->dict:
    return {}

def simple_iteration(data:dict)->dict:
    return {}



