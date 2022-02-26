import sys

def solve(ifile):
    if (ifile == sys.stdin):
        _solve_stdin()
    else:
        _solve_ifile(ifile)


def _solve_stdin():
    print("from stdin!")

def _solve_ifile(ifile):
    print("from ifile!")

