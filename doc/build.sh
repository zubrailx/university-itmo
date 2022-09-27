#!/bin/bash

CURRENT_DIR=`pwd`
SCRIPT_DIR=`dirname $0`

cd $SCRIPT_DIR

# Idk, just some shitty errors when launching for the first time
latexmk -pdf -shell-escape -xelatex -interaction=nonstopmode -outdir=build/ main.tex || \
latexmk -pdf -shell-escape -xelatex -interaction=nonstopmode -outdir=build/ main.tex
mv build/main.pdf ./

cd $CURRENT_DIR
