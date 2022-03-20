from .node import Node, NodeType
from .tokenize import tokenize, Token, Operator
from .variables import _OperatorsBinary, _OperatorsUnary, _Variables


class ParseException(Exception):
    pass


class TokenPointer:
    def __init__(self, tokens: list):
        self.index = 0
        self.tokens = tokens

    def get(self, offset=0):
        if (len(self.tokens) <= self.index + offset):
            return (Token.EOF, Operator.EOF)
        else:
            return self.tokens[self.index + offset]

    def inc(self, value=1):
        self.index += value

    def substr(self, start, end):
        return self.tokens[start:end]


def parse_expression(string: str) -> tuple:
    tokens = tokenize(string.strip())
    token_pointer = TokenPointer(tokens)
    nodes = parse_tokens(token_pointer, 0)
    variables = get_list_of_variables(nodes)
    return (nodes, list(variables))


def get_list_of_variables(node) -> set:
    variables = set()
    if (node is None):
        return variables
    if (node.node_type in [NodeType.BINARY_OPERATOR, NodeType.UNARY_OPERATOR]):
        variables |= get_list_of_variables(node.left)
        variables |= get_list_of_variables(node.right)
    elif (node.node_type == NodeType.VARIABLE):
        if (type(node.center) == str):
            variables.add(node.center)
    return variables


_last_expression = None

def parse_tokens(pointer: TokenPointer, precedence: int = 0):
    k, v = pointer.get()
    if (k == Token.BRACKET and str(v) in '()'):
        left_node = _parse_brackets(pointer)
    elif (k == Token.VARIABLE):
        left_node = _parse_variable(pointer)
    elif (k == Token.NUMBER):
        left_node = _parse_number(pointer)
    elif (k == Token.OPERATOR and v in _OperatorsUnary.keys()):
        left_node = _parse_unary_operator(pointer)
    else:
        raise ParseException(f"Can get the expression of: {pointer.substr(0, pointer.index + 1)} <- this")

    while True:
        k, v = pointer.get()
        if (k == Token.EOF or k == Token.BRACKET):
            break
        if (k == Token.OPERATOR):
            # check unary operator
            bin_node = _parse_binary_operator(pointer, left_node, precedence)
            if (bin_node is None):
                break
            else:
                left_node = bin_node
        else:
            raise ParseException(f"It should be operator instead of: {(k, v)}")
    return left_node


def _parse_brackets(pointer: TokenPointer):
    pointer.inc()
    node = parse_tokens(pointer, 0)
    k, v = pointer.get()
    if not (k == Token.BRACKET and v == ')'):
        raise ParseException(f"Bracket not found: {pointer.substr(0, pointer.index + 1)} <- this")
    pointer.inc()
    return node

def _parse_variable(pointer: TokenPointer):
    k, v = pointer.get()
    if (v in _Variables):
        node = Node(_Variables[v], NodeType.NUMBER)
    else:
        node = Node(v, NodeType.VARIABLE)
    pointer.inc()
    return node

def _parse_number(pointer: TokenPointer):
    k, v = pointer.get()
    pointer.inc()
    return Node(v, NodeType.NUMBER)

def _parse_unary_operator(pointer: TokenPointer):
    k, v = pointer.get()
    pointer.inc()
    l, r = _OperatorsUnary[v][0]
    if (l < r):
        node = parse_tokens(pointer, r)
        return Node(_OperatorsUnary[v][1], NodeType.UNARY_OPERATOR, None, node)
    elif (l > r):
        raise ParseException(f"Right-left operators ane not realized yet.")
    else:
        raise ParseException(f"Operator {(k, v)} has invalid priority: {(l, r)}")

def _parse_binary_operator(pointer: TokenPointer, left_node, precedence):
    k, v = pointer.get()
    l, r = _OperatorsBinary[v][0]
    if (l > precedence):
        pointer.inc()
        right_node = parse_tokens(pointer, r)
        return Node(_OperatorsBinary[v][1], NodeType.BINARY_OPERATOR, left_node, right_node)
    elif (l < precedence):
        return None
    else:
        raise ParseException(f"Operator {(k, v)} has invalid priority: {(l, r)}")
