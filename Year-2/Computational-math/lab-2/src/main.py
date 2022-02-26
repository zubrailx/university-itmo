import sys
import io

import task

def main():
    # disable buffering
    sys.stdout = io.TextIOWrapper(open(sys.stdout.fileno(), "wb", 0), write_through = True)
    
    # if file is in args, then read from it
    if (len(sys.argv) == 2):
        ifile = open(sys.argv[1], "r")
    else:
        ifile = sys.stdin

    # calculate the task
    task.solve(ifile)


if __name__ == "__main__":
    main()

