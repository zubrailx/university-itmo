from modules.parse import Node, CalculationException


def calculate(node: Node, x_dict: dict, rec: int = 0, eps: float = 0.0001, err: float = 0.001):
    if (rec > 3):
        raise CalculationException("Cannot recover from breaking point.")

    x_dict_plus = x_dict.copy()
    x_dict_minus = x_dict.copy()
    for k in x_dict.keys():
        x_dict_plus[k] += eps
        x_dict_minus[k] -= eps

    try:
        try:
            f_x_plus = node.calculate(x_dict_plus)
        except ZeroDivisionError:
            f_x_plus = calculate(node, x_dict_plus, rec + 1)
        try:
            f_x_minus = node.calculate(x_dict_minus)
        except ZeroDivisionError:
            f_x_minus = calculate(node, x_dict_minus, rec + 1)
    except (ZeroDivisionError, CalculationException):
        raise CalculationException("Point %s is breaking point of second kind" % str(x_dict))

    try:
        f_x = node.calculate(x_dict)
    except (CalculationException, ZeroDivisionError):
        f_x = (f_x_plus + f_x_minus) / 2

    try:
        if (((f_x_plus + f_x_minus) / 2) - f_x > err):
            raise CalculationException("Point %s is breaking point of second kind" % str(x_dict))
    except TypeError:
        raise CalculationException("Point %s is breaking point of second kind" % str(x_dict))

    return f_x
