import sys

import labs.lab_1 as lab_1
import labs.lab_2 as lab_2


labs = {
    1: lab_1.solve,
    2: lab_2.solve
}


def solve():
    try:
        path_input = input("Input file: ").strip()
        path_input = "data/task-2/input.json"
        path_output = input("Output file: ").strip()
        lab_number = int(input("Lab number: ").strip())
    except ValueError:
        sys.exit("ValueError: invalid arguments.")

    stream_input = open(path_input, "r") if path_input else sys.stdin
    stream_output = open(path_output, "w") if path_output else sys.stdout

    if (lab_number in labs.keys()):
        labs[lab_number](stream_input, stream_output)
    else:
        sys.exit(f"Lab with number \"{lab_number}\" is not present.")
    
    
if __name__ == "__main__":
    print("\nThis file is the entry point of modules that contains laboratory works.")
    input("Press enter to continue...")
