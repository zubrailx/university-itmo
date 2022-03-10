import math

from .node import *
from modules.util.is_check import is_number, is_alnum, is_alslash


class StringPointer:
    def __init__(self, string):
        self.string = string
        self.index = 0

    def get(self):
        if (len(self.string) > self.index):
            return self.string[self.index]
        else:
            return chr(0)

    def inc(self, value=1):
        self.index += value


# TODO add operations: sin, cos, tan, ctan, log
Operators = {
    "+": (lambda x, y: x + y),
    "-": (lambda x, y: x - y),
    "*": (lambda x, y: x * y),
    "/": (lambda x, y: x / y),
    "^": (lambda x, y: x ** y)
}

Precedences = {
    "+": (1, 2),
    "-": (1, 2),
    "*": (3, 4),
    "/": (3, 4),
    "^": (6, 5)
}

Variables = {
    "\\pi": math.pi,
    "\\e": math.e
}


def parse_expression(string_pointer) -> tuple:
    tokens = tokenize(string_pointer)
    nodes = parse_tokens(tokens, 0)
    variables = get_list_of_variables(nodes)

    return (nodes, list(variables))


def get_list_of_variables(node) -> set:
    variables = set()
    if (node.node_type == Type.OPERATOR):
        variables |= get_list_of_variables(node.left)
        variables |= get_list_of_variables(node.right)
    elif (node.node_type == Type.VARIABLE):
        if (type(node.center) == str):
            variables.add(node.center)
    return variables

# (Type, value)
def tokenize(string):
    string_pointer = StringPointer(string)
    tokens = []
    while (string_pointer.get() != chr(0)):
        # check number
        if (is_number(string_pointer.get()) or string_pointer.get() == '-' and string_pointer.index < len(string_pointer.string) - 1 and is_number(string_pointer.string[string_pointer.index + 1])):
            var_start = string_pointer.index
            dot_cnt = 0
            string_pointer.inc()
            while (is_number(string_pointer.get()) or string_pointer.get() == '.'):
                if (string_pointer.get() == '.'):
                    dot_cnt += 1
                string_pointer.inc()
            assert(dot_cnt <= 1 and "Can't parse number")
            token = (Type.NUMBER, float(string_pointer.string[var_start:string_pointer.index]))
            tokens.append(token)
        # check operator
        elif (string_pointer.get() in Operators.keys()):
            token = (Type.OPERATOR, string_pointer.get())
            string_pointer.inc()
            tokens.append(token)
        # check brackets
        elif (string_pointer.get() in "()"):
            token = (Type.BRACKET, string_pointer.get())
            string_pointer.inc()
            tokens.append(token)
        # check variable
        elif (is_alslash(string_pointer.get())):
            var_start = string_pointer.index
            string_pointer.inc()
            while (is_alnum(string_pointer.get()) or string_pointer.get() == '_'):
                string_pointer.inc()
            token = (Type.VARIABLE, string_pointer.string[var_start:string_pointer.index])
            tokens.append(token)
        else:
            string_pointer.inc()
    tokens.append((Type.EOF, None))
    return tokens


def parse_tokens(tokens, precedence: int = 0):
    left_type, left_value = tokens.pop(0)
    if (left_type == Type.BRACKET and left_value == '('):
        left_node = parse_tokens(tokens, 0)
        k, v = tokens.pop(0)
        assert (k == Type.BRACKET and v == ')')
    elif (left_type == Type.VARIABLE):
        if (left_value in Variables):
            left_node = Node(Variables[left_value], Type.NUMBER)
        else:
            left_node = Node(left_value, Type.VARIABLE)
    else:
        assert (left_type == Type.NUMBER)
        left_node = Node(left_value, Type.NUMBER)

    while True:
        operator_type, operator = tokens[0]
        if (operator_type == Type.EOF or operator_type == Type.BRACKET):
            break
        assert (operator_type == Type.OPERATOR)

        l, r = Precedences[operator]
        assert (l != precedence and "Undefined behaviour")
        if (l < precedence):
            break
        elif (l > precedence):
            tokens.pop(0)
            right_node = parse_tokens(tokens, r)
            left_node = Node(Operators[operator], Type.OPERATOR, left_node, right_node)
    return left_node
