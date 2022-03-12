from enum import Enum

from .variables import _OperatorsUnaryString, _OperatorsBinaryString
from modules.util.project_exception import ProjectException


class NodeType(Enum):
    BINARY_OPERATOR = 1
    UNARY_OPERATOR = 2
    VARIABLE = 3
    NUMBER = 4


class Node:
    def __init__(self, center, node_type: NodeType, left=None, right=None):
        # self.parent = parent
        self.left = left
        self.center = center
        self.right = right
        self.node_type = node_type

    def calculate(self, var_dict):
        # number
        if (self.node_type == NodeType.NUMBER):
            return self.center
        # variable
        elif (self.node_type == NodeType.VARIABLE):
            if (type(self.center) == str):
                if not (self.center in var_dict.keys()):
                    raise ProjectException(f"Variable '{self.center}' not found in dictionary.")
                return var_dict[self.center]
        # binary operator
        elif (self.node_type == NodeType.BINARY_OPERATOR):
            return self.center(self.left.calculate(var_dict), self.right.calculate(var_dict))
        # unary operator
        elif (self.node_type == NodeType.UNARY_OPERATOR):
            if (self.left is not None):
                return self.center(self.left.calculate(var_dict))
            else:
                return self.center(self.right.calculate(var_dict))
        else:
            raise ProjectException("Cannot calculate this expression")
        return 0

    def __str__(self):
        if (self.node_type == NodeType.NUMBER or self.node_type == NodeType.VARIABLE):
            return str(self.center)
        elif (self.node_type == NodeType.BINARY_OPERATOR):
            return f"({self.left} {_OperatorsBinaryString[self.center]} {self.right})"
        elif (self.node_type == NodeType.UNARY_OPERATOR):
            if (self.right is not None):
                return f"({_OperatorsUnaryString[self.center]}{self.right})"
            else:
                return f"({self.right}{_OperatorsUnaryString[self.center]})"
        else:
            raise ProjectException(f"Cannot __str__ this expression: {self}")
