import sys
import io

import labs as lab


def main():
    # disable buffering
    sys.stdout = io.TextIOWrapper(open(sys.stdout.fileno(), "wb", 0), write_through=True)
    # if file is in args, then read from it
    # file_input = open(sys.argv[1], "r") if len(sys.argv) == 2 else sys.stdin
    # file_output = open(sys.argv[2], "r") if len(sys.argv) == 3 else sys.stdout

    # solve the lab
    lab.solve()


if __name__ == "__main__":
    main()
