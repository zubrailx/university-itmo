from modules.parse import tokenize, parse


while True:
    expression = input("ENTER: ")
    expression = parse.parse_expression(expression)
    print(expression)

