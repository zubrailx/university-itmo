def is_char_alpha(char):
    return (char >= 'a' and char <= 'z') or (char >= 'A' and char <= 'Z')

def is_char_number(char):
    return (char >= '0' and char <= '9')

def is_char_alnum(char):
    return (is_char_alpha(char) or is_char_number(char))

def is_char_blank(char):
    return (char == ' ' or char == '\t' or char == '\n')


def is_word_uint(string: str):
    return all([is_char_number(c) for c in string])
