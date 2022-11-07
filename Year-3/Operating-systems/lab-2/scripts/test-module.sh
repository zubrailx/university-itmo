#!/bin/bash

set -u

MODULE=$1
BASE_NAME=`basename $MODULE`
MODULE_NAME=${BASE_NAME%.*}

sudo dmesg -C
sudo insmod $MODULE
sudo rmmod $MODULE_NAME
dmesg
