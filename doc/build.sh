#!/bin/bash

CURRENT_DIR=`pwd`
SCRIPT_DIR=`dirname $0`

cd $SCRIPT_DIR

# Idk, just some shitty errors when launching for the first time
latexmk -shell-escape -xelatex -interaction=nonstopmode -outdir=build/ main.tex || \
latexmk -shell-escape -xelatex -interaction=nonstopmode -outdir=build/ main.tex

cd $CURRENT_DIR
