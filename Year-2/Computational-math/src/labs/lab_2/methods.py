from copy import copy

from modules.equation import parse
from modules.equation.derivative import grad, node_flatten
from modules.matrix import Matrix
from modules.util.project_exception import ProjectException
from modules.util.color import Color, color_string


def split_half(data: dict) -> dict:
    node_root, var_list = parse.parse_expression(data["equation"][0])
    if (len(var_list) != 1):
        raise ProjectException(color_string(Color.RED, "ERROR >> Amount of variables should be equal to 1"))
    try:
        range_min = data["data"]["range_min"]
        range_max = data["data"]["range_max"]
        iterations = data["data"]["iterations"]
    except KeyError:
        raise ProjectException(color_string(Color.RED,
                                            "Key not found!(should be present range_min, range_max, iterations)"))
    fval_range_min = node_root.calculate({var_list[0]: range_min})
    fval_range_max = node_root.calculate({var_list[0]: range_max})
    if (sum([fval_range_min > 0, fval_range_max > 0]) != 1):
        return {"error": "Invalid arguments. Function values of borders are same sigh"}
    for _ in range(iterations):
        range_med = (range_max + range_min) / 2
        fval_range_med = node_root.calculate({var_list[0]: range_med})
        if (sum([fval_range_med > 0, fval_range_min > 0]) == 1):
            range_max = range_med
        else:
            range_min = range_med
    return {"result": {"range_min": range_min, "range_max": range_max}}


def tangent(data: dict) -> dict:
    node, var_list = parse.parse_expression(data["equation"][0])
    if (len(var_list) != 1):
        raise ProjectException(color_string(Color.RED, "ERROR >> Amount of variables should be equal to 1"))
    try:
        x_0 = data["data"]["x_0"]
        iterations = data["data"]["iterations"]
    except KeyError:
        raise ProjectException(color_string(Color.RED, "Key not found!(should be present x_0)"))
    f = node.calculate({var_list[0]: x_0})
    f_ll = grad(grad(node_flatten(node, {"x": x_0}, "x")))(x_0)

    if (f * f_ll <= 0):
        return {"error": "Invalid arguments. Derivative'' * func <= 0. Iteration process doesn't converge"}
    x_prev = x_0
    for _ in range(iterations):
        f_x = node.calculate({var_list[0]: x_prev})
        f_x_l = grad(node_flatten(node, {"x": x_prev}, "x"))(x_prev)
        x_prev -= f_x / f_x_l

    return {"result": x_prev}


def simple_iteration(data: dict) -> dict:
    equation_list = data["equation"]
    node_list = []
    try:
        iterations = data["data"]["iterations"]
        x0_dict = data["data"]["x_0"]
    except KeyError:
        raise ProjectException(color_string(Color.RED, "Key not found!(should be present x_0)"))
    x0_dict_keys = list(x0_dict.keys())
    assert(len(x0_dict_keys) == len(equation_list))

    for i in range(len(equation_list)):
        equation_list[i] = "-1 * (" + equation_list[i] + "-" + x0_dict_keys[i] + ")"
        n, v = parse.parse_expression(equation_list[i])
        node_list.append(n)
    matrix_phi = Matrix(len(equation_list), len(x0_dict_keys))
    # check convergence
    for i in range(matrix_phi.rows):
        for j in range(matrix_phi.columns):
            matrix_phi[i][j] = grad(node_flatten(node_list[i], x0_dict, x0_dict_keys[j]))(x0_dict[x0_dict_keys[j]])
    norm = max([sum([matrix_phi[i][j] for j in range(matrix_phi.columns)]) for i in range(matrix_phi.rows)])
    if (norm >= 1):
        return {"error" : "This equations for those basic arguments are not convergent"}

    x0_dict_prev = copy(x0_dict)
    for _ in range(iterations):
        for i in range(len(node_list)):
            x0_dict[x0_dict_keys[i]] = node_list[i].calculate(x0_dict_prev)
        x0_dict_prev = copy(x0_dict)

    return {"values": x0_dict_prev}
