import math
from .tokenize import Operator


_OperatorsBinary = {
    Operator.SUM: ((1, 2), lambda x, y: x + y),
    Operator.SUB: ((1, 2), lambda x, y: x - y),
    Operator.MUL: ((3, 4), lambda x, y: x * y),
    Operator.DIV: ((3, 4), lambda x, y: x / y),
    Operator.POW: ((6, 5), lambda x, y: x ** y)
}

# if I want to add operator that works from right to left I need to create in structure node pointer to parent and
# create variable that contains the pointer to the latest bracket and variable, so I can make a decorator and replace
# old object with new that contains the old. So I don't need unary operators that work right to left :)
# TODO add operator: log(x, base)
_OperatorsUnary = {
    Operator.SUB: ((1, 2), lambda x: -x),
    Operator.SIN: ((1, 2), lambda x: math.sin(x)),
    Operator.COS: ((1, 2), lambda x: math.cos(x)),
    Operator.TAN: ((1, 2), lambda x: math.tan(x)),
    Operator.COT: ((1, 2), lambda x: 1 / math.tan(x)),
    Operator.LOGE: ((1, 2), lambda x: math.log(x, math.e)),
    Operator.LOG2: ((1, 2), lambda x: math.log2(x)),
    Operator.LOG10: ((1, 2), lambda x: math.log10(x))
}

_Variables = {
    "\\pi": math.pi,
    "\\e": math.e
}


_OperatorsBinaryString = dict()
_OperatorsUnaryString = dict()


def _operator_init_string():
    for k, v in _OperatorsBinary.items():
        _OperatorsBinaryString[v[1]] = k.value
    for k, v in _OperatorsUnary.items():
        _OperatorsUnaryString[v[1]] = k.value


# initialization
_operator_init_string()
