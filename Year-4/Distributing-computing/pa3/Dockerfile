FROM ubuntu:14.04

RUN apt-get update && apt-get install -y \
    clang \
    build-essential \
    cmake \
    gdb \
    valgrind

WORKDIR /app

COPY . /app

RUN make build

CMD ["./a.out"]
