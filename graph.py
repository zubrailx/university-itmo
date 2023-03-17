import matplotlib.pyplot as plt

import pandas as pd
from pandas import DataFrame
from pandas import Series

import numpy as np

def insert_one():
    fname = "./src/test/data/unit/func.Function.csv"
    df = pd.read_csv(fname, delimiter=",", header=None, skiprows=1)
    x = df[0]
    y = df[1]

    fig, ax = plt.subplots()
    ax.scatter(x, y)
    plt.title("Function scatter")
    plt.ylim(-1000, 1000)
    plt.savefig("./build/function-scatter.png")
    plt.clf()

    fig, ax = plt.subplots()
    ax.plot(x, y)
    plt.title("Function plot")
    plt.ylim(-1000, 1000)
    plt.savefig("./build/function-plot.png")
    plt.clf()


insert_one()
