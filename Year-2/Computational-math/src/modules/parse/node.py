from enum import Enum


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
        if (self.node_type == NodeType.NUMBER):
            return self.center
        elif (self.node_type == NodeType.VARIABLE):
            if (type(self.center) == str):
                assert (self.center in var_dict.keys())
                return var_dict[self.center]
        elif (self.node_type == NodeType.BINARY_OPERATOR):
            return self.center(self.left.calculate(var_dict), self.right.calculate(var_dict))
        elif (self.node_type == NodeType.UNARY_OPERATOR):
            if (self.left is not None):
                return self.center(self.left.calculate(var_dict))
            else:
                return self.center(self.right.calculate(var_dict))
        else:
            raise Exception("Cannot calculate this expression")
        return 0
