def is_uint_word(string):
    return all([is_number(char) for char in string])

def is_number_word(s):
    if s[0] == '-':
        s = s[1:]
    return s.replace('.', '', 1).isdigit()


def is_alpha(char):
    return (char >= 'a' and char <= 'z') or (char >= 'A' and char <= 'Z')

def is_number(char):
    return (char >= '0' and char <= '9')

def is_alslash(char):
    return (is_alpha(char) or (char == '\\'))

def is_alnum(char):
    return (is_alpha(char) or is_number(char))

def is_blank(char):
    return (char == ' ' or char == '\t' or char == '\n')
