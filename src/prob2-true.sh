#!/bin/bash

nasm -g prob2-true.asm -felf64 -o prob2-true.o
ld -o prob2-true prob2-true.o
