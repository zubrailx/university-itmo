def is_uint(string):
    return all([char.isdigit() for char in string])


def is_number(s):
    if s[0] == '-':
        s = s[1:]
    return s.replace('.', '', 1).isdigit()

