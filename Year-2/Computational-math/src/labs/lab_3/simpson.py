from modules.util import ProjectException
from modules.equation import calculate, node_flatten, grad, sgrad, sympy_flatten, seval
from modules.parse import Node, CalculationException

_epsilon1 = 1 / 2 ** 53

def simpson(data):
    try:
        equation_lst = data["equation"]     # (equation, vars)
        range_min = data["data"]["range_min"]
        range_max = data["data"]["range_max"]
        step = data["data"]["step"]
    except KeyError:
        raise ProjectException("Not all arguments are present")
    # validation
    if (range_min >= range_max):
        raise ProjectException("range_min >= range_max")
    if (step <= 0):
        raise ProjectException("step <= 0")
    if (len(equation_lst) == 0):
        raise ProjectException("Equation list is empty")
    # calculation
    result_lst = list()
    for i in range(len(equation_lst)):
        equation = equation_lst[i][0]

        var_lst = equation_lst[i][1]
        if (len(var_lst) != 1):
            raise ProjectException(f"In equation {str(equation)} var_list != 1")

        result_lst.append(dict())
        result_lst[i]["result"] = _calculate_integral(equation, range_min, range_max, step, var_lst)
        result_lst[i]["r1"] = _calculate_r1(range_min, range_max)
        result_lst[i]["r2"] = _calculate_r2_sympy(equation, range_min, range_max, step, var_lst)
        result_lst[i]["r"] = result_lst[i]["r1"] + result_lst[i]["r2"]
    return result_lst


def _calculate_integral(equation: Node, range_min, range_max, step, var_lst):
    y0 = calculate(equation, {var_lst[0]: range_min})
    yn = calculate(equation, {var_lst[0]: range_max})
    n = int((range_max - range_min) / step)
    sigma1 = sum([calculate(equation, {var_lst[0]: range_min + step * i}) for i in range(1, n, 2)])
    sigma2 = sum([calculate(equation, {var_lst[0]: range_min + step * i}) for i in range(2, n, 2)])
    return (step / 3) * (y0 + yn + 4 * sigma1 + 2 * sigma2)

    
def _calculate_r1(range_min, range_max):
    return (range_max - range_min) * _epsilon1

# invalid calculation cuz many errors
def _calculate_r2(equation: Node, range_min, range_max, step, var_lst):
    # finding M4
    f_l4 = grad(grad(grad(grad(node_flatten(equation, {var_lst[0]: None}, var_lst[0])))))
    n = int((range_max - range_min) / step)
    y_l4 = float("-inf")
    for i in range(n):
        try:
            y_cur_l4 = f_l4(range_min + step * i)
            if (y_cur_l4 > y_l4):
                y_l4 = y_cur_l4
        except CalculationException:
            continue
    if (y_l4 == float("-inf")):
        return float("NaN")
    return (range_max - range_min) * (step ** 4) / 180 * y_l4

def _calculate_r2_sympy(equation: Node, range_min, range_max, step, var_lst):
    sympy_expr, sympy_var = sympy_flatten(equation, var_lst)
    for _ in range(4):
        sympy_expr = sgrad(sympy_expr, sympy_var[0])
    f_l4 = sympy_expr
    n = int((range_max - range_min) / step)
    y_l4 = float("-inf")
    for i in range(n):
        y_cur_l4 = float(seval(f_l4, {sympy_var[0]: range_min + step * i}))
        if (y_cur_l4 > y_l4):
            y_l4 = y_cur_l4
    if (y_l4 == float("-inf")):
        return float("NaN")
    return (range_max - range_min) * (step ** 4) / 180 * y_l4
