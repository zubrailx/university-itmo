import matplotlib.pyplot as plt

import pandas as pd
from pandas import DataFrame
from pandas import Series

import math
import numpy as np


x = [ i for i in range(1, 10000) ]
y = [ math.ceil(xi / 1480) for xi in x ]


def insert_one():
    fig, ax = plt.subplots()
    ax.scatter(x, y)
    plt.title("ICMP")
    plt.xlabel("size, bytes")
    plt.ylabel("fragments, count")
    plt.savefig("./icmp.png")
    plt.clf()

insert_one()
