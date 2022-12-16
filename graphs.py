import matplotlib.pyplot as plt

import pandas as pd
from pandas import DataFrame
from pandas import Series

import numpy as np

def insert_one():
    fname = "tmp/insert_one.txt"
    df = pd.read_csv(fname, delimiter=";", header=None)
    x = df[0]
    y = df[1]

    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.set_xlabel("Amount, units")
    ax.set_ylabel("Time, us")
    plt.title("Insert single 100k times time")
    plt.savefig("tmp/insert_one.png")
    plt.clf()

def insert_filesize():
    fname = "tmp/insert_one.txt"
    df = pd.read_csv(fname, delimiter=";", header=None)
    x = df[0]
    y = df[2]

    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.set_xlabel("Amount, units")
    ax.set_ylabel("Size, bytes")
    plt.title("Insert single 100k times file size")
    plt.savefig("tmp/insert_filesize.png")
    plt.clf()

    
def select():
    fname = "tmp/select.txt"
    df = pd.read_csv(fname, delimiter=";", header=None)
    x = df[0]
    y = df[1]

    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.set_xlabel("Operations, idx")
    ax.set_ylabel("Time, us")
    plt.title("Insert 10k elements and select x 100")
    plt.savefig("tmp/select.png")
    plt.clf()

def select_optional():
    fname = "tmp/select_optional.txt"
    df = pd.read_csv(fname, delimiter=";", header=None)
    x = df[0]
    y = df[1]

    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.set_xlabel("Operations, idx")
    ax.set_ylabel("Time, us")
    plt.title("Insert 10k & select(table1.name = 'nikita' and table1.weight > 80) x 100")
    plt.savefig("tmp/select_optional.png")
    plt.clf()

def update_optional():
    fname = "tmp/update_optional.txt"
    df = pd.read_csv(fname, delimiter=";", header=None)
    x = df[0]
    y = df[1]

    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.set_xlabel("Operations, idx")
    ax.set_ylabel("Time, us")
    plt.title("Insert 10k & update(table1.name = 'nikita' and table1.weight > 80) x 100")
    plt.savefig("tmp/update_optional.png")
    plt.clf()

def delete_optional():
    fname = "tmp/delete_optional.txt"
    df = pd.read_csv(fname, delimiter=";", header=None)
    x = df[0]
    y = df[1]

    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.set_xlabel("Operations, idx")
    ax.set_ylabel("Time, us")
    plt.title("Insert 10k & delete(table1.name = 'nikita' and table1.weight > 80) x 100")
    plt.savefig("tmp/delete_optional.png")
    plt.clf()

def size_delete_insert():
    fname = "tmp/size_delete_insert.txt"
    df = pd.read_csv(fname, delimiter=";", header=None)
    x = df[0]
    y = df[1]

    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.set_xlabel("Operations, idx")
    ax.set_ylabel("Size, bytes")
    plt.title("Insert 1kk elements, delete 1/4, insert 500k")
    plt.savefig("tmp/size_delete_insert.png")
    plt.clf()
    

if __name__ == "__main__":
    insert_one()
    insert_filesize()
    select()
    select_optional()
    update_optional()
    delete_optional()
    size_delete_insert()
