from modules.parse import Node, CalculationException


def calculate(node: Node, x_dict: dict, eps: float = 0.0001, err: float = 0.001):
    x_dict_plus = x_dict.copy()
    x_dict_minus = x_dict.copy()
    for k in x_dict.keys():
        x_dict_plus[k] += eps
        x_dict_minus[k] -= eps
    f_x_plus = node.calculate(x_dict_plus)
    f_x_minus = node.calculate(x_dict_minus)
    try:
        f_x = node.calculate(x_dict)
    except CalculationException:
        f_x = (f_x_plus + f_x_minus) / 2
    if (((f_x_plus + f_x_minus) / 2) - f_x > err):
        raise CalculationException("Point %s is breaking point of second kind" % str(x_dict))
    return f_x
