#!/bin/bash

CURRENT_DIR=`pwd`
SCRIPT_DIR=`dirname $0`

cd $SCRIPT_DIR

# Idk, just some shitty errors when launching for the first time
latexmk -pdf -shell-escape -xelatex -outdir=build/ -interaction=nonstopmode main.tex
mv build/main.pdf ./
