#!/bin/bash

set -u

BASE_DIR=`pwd`
SCRIPT_DIR=$BASE_DIR/`dirname $0`
LINUX_DIR=$1
LIST_REQUIRED="build/ Makefile include/ scripts/ arch/"
SCRIPTS="install-kernel.sh test-module.sh"

cd $LINUX_DIR
make O=build ARCH=x86 -j8 vmlinux modules
for script in $SCRIPTS; do cp $SCRIPT_DIR/$script .; done 

tar cf $BASE_DIR/export-$LINUX_DIR.tar $LIST_REQUIRED $SCRIPTS
# tar czf $BASE_DIR/export-$LINUX_DIR.tar.gz .
rm $SCRIPTS
