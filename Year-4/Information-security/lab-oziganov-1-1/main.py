#!/usr/bin/python3

import argparse
from typing import List, Tuple


ALPHABET = (
    [chr(i) for i in range(ord('A'), ord('z') + 1)] +
    [" ", "!", ";", ":", "?", ",", "-", ".", "—"] +
    [chr(i) for i in range(ord('А'), ord('я') + 1)]
)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-k", "--key", required=True)
    parser.add_argument("ifile")
    parser.add_argument("ofile")
    parser.add_argument("-c", "--encrypt", action="store_true")
    parser.add_argument("-z", "--decrypt", action="store_true")
    parser.add_argument("-a", "--analyze", action="store_true")
    parser.add_argument("-v", "--verbose", action="store_true")
    return parser.parse_args()


def validate_args(args) -> Tuple[bool, str]:
    if args.encrypt == args.decrypt:
        if args.encrypt == False and args.analyze == False:
            return (True, "oneof encrypt or decrypt should be present")
        if args.encrypt == True:
            return (True, "oneof encrypt or decrypt should be present")

    return (False, "")


def encrypt(istr, key):
    n = len(ALPHABET)
    rs = ""

    for i, s in enumerate(istr):
        sai, kai = ALPHABET.index(s), ALPHABET.index(key[i % len(key)])
        ri = (sai + kai) % n
        r = ALPHABET[ri]
        rs += r

    return rs


def decrypt(istr, key):
    n = len(ALPHABET)
    rs = ""

    for i, s in enumerate(istr):
        sai, kai = ALPHABET.index(s), ALPHABET.index(key[i % len(key)])
        ri = (sai - kai) % n
        r = ALPHABET[ri]
        rs += r

    return rs


def analyze(istr) -> List[float]:
    cnts = [0] * len(ALPHABET)
    for a in istr:
        i = ALPHABET.index(a)
        cnts[i] += 1

    freqs = []
    for c in cnts:
        freqs.append(c / len(istr))

    return freqs


def display_analysis(freqs):
    for i, a in enumerate(ALPHABET):
        print(f"({a}, {round(freqs[i], 3)})")

    # kasiski match index
    ksum = 0.0
    for f in freqs:
        ksum += f * f
    print("Kasiski match index: ", round(ksum, 4))


def process(args):
    ifile = open(args.ifile, "r")
    ofile = open(args.ofile, "w")

    istr = "".join(ifile.readlines()).strip()
    ostr = None
    key = args.key

    if args.verbose:
        print(f"read from {args.ifile}:", istr)

    if args.encrypt:
        ostr = encrypt(istr, key)

    if args.decrypt:
        ostr = decrypt(istr, key)

        if args.analyze:
            freqs = analyze(istr)
            display_analysis(freqs)

    if ostr is not None:
        ofile.write(ostr)

    if args.verbose:
        print(f"written to {args.ofile}:", ostr)

    ofile.close()
    ifile.close()


def main():
    args = parse_args()

    err, msg = validate_args(args)
    if err:
        print("error:", msg)
        exit(-1)

    process(args)


if __name__ == "__main__":
    main()
