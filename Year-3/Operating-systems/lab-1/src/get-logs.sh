#!/bin/bash

set -e
set -u

PROC_PID=$1
DELAY=$2
COUNT=$3

PWD_DIR=`pwd`
SCRIPT_DIR=`dirname "$0"`
LOGS_DIR="$SCRIPT_DIR/logs"

DISK=/dev/sda1
NET_INT=veth-t

# if [[ -n ${$PROC_PID+x} ]]; then
if [[ -z "$PROC_PID" || -z "$DELAY" || -z "$COUNT" ]]; then
    >&2 echo "Not all arguments set"
    exit -1
fi

if ! [[ -d "/proc/$PROC_PID" ]]; then
    >&2 echo "Process does not exists"
    exit -1
fi

pidstat -ur -p $PROC_PID $DELAY $COUNT > $LOGS_DIR/os-1-cpu.log &
iostat -d -p $DISK $DELAY $COUNT > $LOGS_DIR/os-1-io.log &
ifstat -t -n -i $NET_INT $DELAY $COUNT > $LOGS_DIR/os-1-net.log &
top -b -H -p $PROC_PID -d $DELAY -n $COUNT > $LOGS_DIR/os-1-state.log
echo "Done."
