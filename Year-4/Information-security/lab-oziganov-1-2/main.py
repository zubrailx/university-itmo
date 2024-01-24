#!/usr/bin/python3

import argparse
from typing import IO, Tuple
from bitarray import bitarray

BLOCK_BITS = 64
KEY_BITS = 56

INIT_V = bitarray()
INIT_V.frombytes(b"\x00\x00\x00\x00\x00\x00\x00\x00")

KEY_PERMUTATION = [
    56, 48, 40, 32, 24, 16,  8,
     0, 57, 49, 41, 33, 25, 17,
     9,  1, 58, 50, 42, 34, 26,
    18, 10,  2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,
     6, 61, 53, 45, 37, 29, 21,
    13,  5, 60, 52, 44, 36, 28,
    20, 12,  4, 27, 19, 11,  3
]

INIT_PERM_TABLE = [
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
    56, 48, 40, 32, 24, 16, 8,  0,
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6
]



KEY_COMPR_PERM = [
    13, 16, 10, 23,  0,  4,
     2, 27, 14,  5, 20,  9,
    22, 18, 11,  3, 25,  7,
    15,  6, 26, 19, 12,  1,
    40, 51, 30, 36, 46, 54,
    29, 39, 50, 44, 32, 47,
    43, 48, 38, 55, 33, 52,
    45, 41, 49, 35, 28, 31
]

EXPANSION_TABLE = [
    31,  0,  1,  2,  3,  4,
     3,  4,  5,  6,  7,  8,
     7,  8,  9, 10, 11, 12,
    11, 12, 13, 14, 15, 16,
    15, 16, 17, 18, 19, 20,
    19, 20, 21, 22, 23, 24,
    23, 24, 25, 26, 27, 28,
    27, 28, 29, 30, 31,  0
]

S_BOXES = [
    # S1
    [14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
     0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
     4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
     15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13],

    # S2
    [15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
     3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
     0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
     13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9],

    # S3
    [10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
     13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
     13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
     1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12],

    # S4
    [7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
     13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
     10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
     3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14],

    # S5
    [2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
     14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
     4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
     11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3],

    # S6
    [12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
     10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
     9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
     4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13],

    # S7
    [4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
     13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
     1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
     6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12],

    # S8
    [13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
     1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
     7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
     2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11],
]

STRAIGHT_PERMUTATION = [
    15, 6, 19, 20, 28, 11, 27, 16,
    0, 14, 22, 25, 4, 17, 30, 9,
    1, 7, 23, 13, 31, 26, 2, 8,
    18, 12, 29, 5, 21, 10, 3, 24
]

FINAL_PERMUTATION = [
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25,
    32,  0, 40,  8, 48, 16, 56, 24
]

ROUND_SHIFTS = [1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1]

assert len(INIT_V) == BLOCK_BITS


CODE_DCT = {}
for i in range(256):
    br = format(i, "b")
    CODE_DCT[i] = bitarray("0" * (8 - len(br)) + br)

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-k", "--key", required=True, help="hex string")
    parser.add_argument("ifile", help="input file")
    parser.add_argument("ofile", help="output file")
    parser.add_argument("-c", "--encrypt", action="store_true")
    parser.add_argument("-z", "--decrypt", action="store_true")
    parser.add_argument("-v", "--verbose", action="store_true")

    args = parser.parse_args()
    return args


def validate_args(args) -> Tuple[bool, str]:
    if args.encrypt == args.decrypt:
        if args.encrypt == False:
            return (True, "oneof encrypt or decrypt should be present")
        if args.encrypt == True:
            return (True, "oneof encrypt or decrypt should be present")

    return (False, "")


def to_hex(data: bitarray) -> str:
    return format(int(data.to01(), 2), f'0>{len(data) // 4}X')


def split_half(data: bitarray, data_size):
    size = data_size // 2
    left, right = bitarray(size), bitarray(size)
    left[:size] = data[:size]
    right[:size] = data[size:]
    return (left, right)


def shift_off(data: bitarray, off : int):
    s = len(data)
    out = bitarray(s)
    out[:s-off] = data[off:]
    out[s-off:] = data[:off] 
    return out


def permutate(table, data: bitarray):
    out = bitarray(len(table))
    for i, j in enumerate(table):
        out[i] = data[j]
    return out


def initial_permutation(data: bitarray):
    return permutate(INIT_PERM_TABLE, data)


def compression_permutation(key: bitarray):
    return permutate(KEY_COMPR_PERM, key)


def expansion_permutation(data: bitarray):
    return permutate(EXPANSION_TABLE, data)


def straight_permutation(data: bitarray):
    return permutate(STRAIGHT_PERMUTATION, data)


def final_permutation(left: bitarray, right: bitarray):
    out = bitarray(BLOCK_BITS)
    for i, e in enumerate(FINAL_PERMUTATION):
        out[i] = right[e - 32] if e > 31 else left[e]
    return out


def des_key_permutation(key: bitarray):
    return permutate(KEY_PERMUTATION, key)


def key_shift(i: int, pkey: bitarray):
    shift = ROUND_SHIFTS[i]
    s = len(pkey)
    out = bitarray(s)
    hs = s // 2

    out[:hs] = shift_off(pkey[:hs], shift)
    out[hs:] = shift_off(pkey[hs:],shift)


    return out


def sbox_transformation(data: bitarray):
    out = bitarray(32)
    for i in range(8):
        lsi = i * 6
        rsi = (i + 1) * 6
        row = int((data[lsi] << 1) + data[rsi - 1])
        column = int(data[lsi + 1: rsi - 1].to01(), 2)
        v = S_BOXES[i][row * 16 + column]
        val = align_left(bitarray(format(v, 'b')), 4)
        out[i * 4 : (i + 1) * 4] = val
    return out


def des_keys(key: bitarray):
    keys = []
    for i in range(0, 16, 1):
        key = key_shift(i, key)
        key_c = compression_permutation(key)
        keys.append(key_c)

    return keys


def des_key64(key: bitarray):
    key64 = bitarray(64)
    key64.setall(0)

    for i in range(8):
        ki = key[i * 7: (i + 1) * 7]
        k64l = i * 8
        k64r = (i + 1) * 8
        key64[k64l: k64r - 1] = ki
        if ki.count(1) % 2 == 0:
            key64[k64r - 1] = 1

    return key64


def des_encrypt(key: bitarray, data: bitarray, verbose=False) -> bitarray:
    key64 = des_key64(key)
    key56 = des_key_permutation(key64) # get 56-bit key
    keys = des_keys(key56)

    ip = initial_permutation(data)
    lp, rp = split_half(ip, BLOCK_BITS)

    for k in keys:
        ext_r = expansion_permutation(rp)
        xor_r = ext_r ^ k
        sb_r = sbox_transformation(xor_r)
        f_r_k = straight_permutation(sb_r)
        r = lp ^ f_r_k 
        l = rp
        lp, rp = l, r

    return final_permutation(rp, lp)


def des_decrypt(key, data: bitarray, verbose=False):
    key64 = des_key64(key)
    key56 = des_key_permutation(key64)
    keys = des_keys(key56)
    keys.reverse()

    # same as above
    ip = initial_permutation(data)
    lp, rp = split_half(ip, BLOCK_BITS)

    for k in keys:
        ext_r = expansion_permutation(rp)
        xor_r = ext_r ^ k
        sb_r = sbox_transformation(xor_r)
        f_r_k = straight_permutation(sb_r)
        r = lp ^ f_r_k 
        l = rp
        lp, rp = l, r

    return final_permutation(rp, lp)


def cbc_encrypt(key: bitarray, data: bitarray, enc_func, verbose) -> bitarray:
    assert len(data) % BLOCK_BITS == 0

    res = bitarray(len(data))
    prev = INIT_V.copy()

    for i in range(len(data) // BLOCK_BITS):
        lsi = i * BLOCK_BITS
        rsi = (i + 1) * BLOCK_BITS
        prev = enc_func(key, data[lsi:rsi] ^ prev, verbose)
        res[lsi:rsi] = prev

    return res


def cbc_decrypt(key: bitarray, data: bitarray, dec_func, verbose) -> bitarray:
    assert len(data) % BLOCK_BITS == 0

    prev = INIT_V.copy()
    res = bitarray(len(data))

    for i in range(len(data) // BLOCK_BITS):
        lsi = i * BLOCK_BITS
        rsi = (i + 1) * BLOCK_BITS
        res[lsi:rsi] = dec_func(key, data[lsi:rsi], verbose) ^ prev
        prev = data[lsi:rsi]

    return res


def process(data, key, verbose, encrypt, decrypt):
    out = None

    if encrypt:
        out = cbc_encrypt(key, data, des_encrypt, verbose)

    if decrypt:
        out = cbc_decrypt(key, data, des_decrypt, verbose)

    assert out is not None
    return out


def align_right(data: bitarray, alignment: int) -> bitarray:
    alignment = (alignment - len(data) % alignment) % alignment
    out = bitarray(len(data) + alignment)
    out[0:len(data)] = data
    out[len(data):] = 0
    return out


def align_left(data: bitarray, alignment: int) -> bitarray:
    alignment = (alignment - len(data) % alignment) % alignment
    out = bitarray(len(data) + alignment)
    out[0:alignment] = 0
    out[alignment:] = data
    return out

def main():
    args = parse_args()
    err, msg = validate_args(args)
    if err:
        print("error:", msg)
        exit(-1)

    ifile = open(args.ifile, "rb")
    ofile = open(args.ofile, "wb")

    # read hex key
    key = bitarray(format(int(args.key, 16), "b"))
    if len(key) > KEY_BITS:
        print(f"error: max {KEY_BITS} bit key, got {len(key)}")
        exit(-1)
    key = align_left(key, KEY_BITS)

    if args.verbose:
        print("key: ", to_hex(key))

    # read data
    istr = ifile.read()
    if args.verbose:
        print(f"read from {args.ifile}:", istr)

    idata = bitarray()
    idata.encode(CODE_DCT, istr)
    idata = align_right(idata, BLOCK_BITS)


    odata = process(
        idata,
        key,
        args.verbose,
        args.encrypt,
        args.decrypt,
    )

    ostr = bytes(odata.decode(CODE_DCT))
    ofile.write(ostr)

    if args.verbose:
        print(f"written to {args.ofile}:", ostr)

    ofile.close()
    ifile.close()


def test():
    key = b'\xff\xff\xff\xff\xff\xff\xff\xff'
    data = 'hellowor'

    idata = bitarray()
    idata.encode(CODE_DCT, data)

    ikey = bitarray()
    ikey.frombytes(key)

    print("data:", idata, len(idata))
    print("key: ", to_hex(ikey), len(ikey))

    out = des_encrypt(ikey, idata)
    print("my_out: ", to_hex(out))

    inp = des_decrypt(ikey, out)
    print("res: ", to_hex(inp))


if __name__ == "__main__":
    main()


