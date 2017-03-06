#!/bin/bash


# pull in my pretty colors
. ~/bin/colors.sh

#globals
BUILD_DIR=/home/kgoldstein/dev/eclipse/msc2p1
BUILD_LOG=$BUILD_DIR/build.log
BUILD_EXE=$BUILD_DIR/msc2p1

# first, take out the objects
objects=`find $BUILD_DIR -name "*.o" -print`
for object in objects
do
    echoRed "Deleting:$file"
done


# next, take out the exectuable
C2P_BIN=msc2p1
exec=`find $BUILD_DIR -name "$C2P_BIN"`
for file in $exec
do
    echoRed "Deleting:$file"
done



