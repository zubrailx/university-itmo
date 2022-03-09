from enum import Enum


class Type(Enum):
    OPERATOR = 0
    VARIABLE = 1
    EOF = 2
    BRACKET = 3
    NUMBER = 4

class Node:
    def __init__(self, center, node_type, left=None, right=None):
        self.left = left
        self.center = center
        self.right = right
        self.node_type = node_type

    def __str__(self):
        string = f"({self.left}, {self.center}, {self.right})"
        return string
    
    def calculate(self, var_dict):
        if (self.node_type == Type.NUMBER):
            return self.center
        elif (self.node_type == Type.VARIABLE):
            if (type(self.center) == str):
                assert (self.center in var_dict.keys())
                return var_dict[self.center]
        elif (self.node_type == Type.OPERATOR):
            return self.center(self.left.calculate(var_dict), self.right.calculate(var_dict))
        else:
            raise Exception("Cannot calculate this expression")
        return 0
            
