from enum import Enum 

class Color(Enum):  # You may need to change color settings
    RED = '\033[31m'
    ENDC = '\033[m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    BLUE = '\033[34m'

def color_string(color: Color, string):
    return color.value + string + Color.ENDC.value
