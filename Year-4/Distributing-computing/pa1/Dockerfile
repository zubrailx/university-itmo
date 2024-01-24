FROM ubuntu:14.04

RUN apt-get update && apt-get install -y \
    clang \
    build-essential \
    cmake \
    gdb \
    valgrind

WORKDIR /app

COPY . /app

RUN clang -std=c99 -Wall -pedantic -g *.c

CMD ["./a.out"]
