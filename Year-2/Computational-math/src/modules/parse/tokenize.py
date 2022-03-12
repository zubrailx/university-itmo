from enum import Enum
from modules.util.is_check import is_char_number, is_char_alnum, is_char_alpha, is_char_blank


class TokenizeException(Exception):
    pass


class Token(Enum):
    EOF = 0
    OPERATOR = 1
    VARIABLE = 2
    NUMBER = 3
    BRACKET = 4
    

class Operator(Enum):
    SUM = "+"
    SUB = "-"
    DIV = "/"
    POW = "^"
    MUL = "*"
    EOF = "\x00"

    def __str__(self):
        return self.value


class StringPointer:
    def __init__(self, string):
        self.string = string
        self.index = 0

    def get(self, offset=0):
        if (len(self.string) <= self.index + offset):
            return chr(0)
        else:
            return self.string[self.index + offset]

    def set(self, index):
        self.index = index

    def inc(self, value=1):
        self.index += value

    def is_end(self):
        return self.index >= len(self.string)

    def substr(self, start, end):
        return self.string[start:end]


def tokenize(string):   # -> (Type, value)
    pointer = StringPointer(string)
    tokens = []
    while (not pointer.is_end()):
        # check operator (should have higher priority than number, cuz x - 5 will be x <none> (-5))
        length = _is_operator(pointer)
        if (length):
            _add_token(tokens, pointer, Token.OPERATOR, length)
            continue
        # check brackets
        length = _is_bracket(pointer)
        if (length):
            _add_token(tokens, pointer, Token.BRACKET, length)
            continue
        # check variable
        length = _is_variable(pointer)
        if (length):
            _add_token(tokens, pointer, Token.VARIABLE, length)
            continue
        # check number
        length = _is_number(pointer)
        if (length):
            _add_token(tokens, pointer, Token.NUMBER, length)
            continue
        # skip blank character
        length = _is_blank(pointer)
        if (length):
            pointer.inc(_is_blank(pointer))
            continue
        raise TokenizeException(f"TokenizeException: Unknown symbol\n{pointer.string}\n{' ' * pointer.index + '~'}")
    return tokens


def _add_token(tokens: list, pointer: StringPointer, typo: Token, length: int):
    if (typo == Token.OPERATOR):
        substr = pointer.substr(pointer.index, pointer.index + length)
        token = None
        for e in Operator:
            if e.value == substr:
                token = (typo, e)
                break
    else:
        token = (typo, pointer.substr(pointer.index, pointer.index + length))
    tokens.append(token)
    pointer.inc(length)


def _is_operator(pointer: StringPointer) -> int:
    return int(pointer.get() in [operator.value for operator in Operator])

def _is_variable(pointer: StringPointer) -> int:
    length = 0
    if (is_char_alpha(pointer.get()) or (pointer.get() == '\\' and is_char_alpha(pointer.get(1)))):
        while True:
            length += 1
            if not (is_char_alnum(pointer.get(length)) or pointer.get(length) == '_'):
                break
    return length

def _is_bracket(pointer: StringPointer) -> int:     
    return int(pointer.get() in "()")

def _is_number(pointer: StringPointer) -> int:
    length = 0
    dot_count = int(pointer.get() == '.')
    if (is_char_number(pointer.get()) or ((dot_count == 1 or pointer.get() == '-') and is_char_number(pointer.get(1)))):
        while True:
            length += 1
            if (is_char_number(pointer.get(length))):
                continue
            elif (pointer.get(length) == '.'):
                dot_count += 1
                if (dot_count > 1):
                    break
            else:
                break
    return length

def _is_blank(pointer: StringPointer) -> int:
    length = 0
    while (is_char_blank(pointer.get(length))):
        length += 1
    return length
