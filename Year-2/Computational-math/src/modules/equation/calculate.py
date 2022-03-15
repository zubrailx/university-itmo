from modules.parse import Node, CalculationException


def calculate(node: Node, x_dict: dict, eps: float = 0.000001):
    try:
        value = node.calculate(x_dict)
    except CalculationException:
        try:
            x_dict_plus = x_dict.copy()
            x_dict_minus = x_dict.copy()
            for k, _ in x_dict_plus:
                x_dict_plus[k] += eps
                x_dict_minus[k] -= eps
            value = (node.calculate(x_dict_plus) + node.calculate(x_dict_minus)) / 2
        except CalculationException:
            raise CalculationException("Function cannot be calculated (f(x) +- eps is not defined)")
    return value

