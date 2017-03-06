#!/bin/bash

# my dirs
BUILD_DIR=/home/kgoldstein/dev/eclipse/msc2p1

echo $BUILD_DIR/msc2p1 \
-i 127.0.0.1:7000,127.0.0.1:7001 \
-o 127.0.0.1:5555 \
-m 127.0.0.1:5001 \
--ponythreads 1 \
--ponypinasio \
--ponynoblock \
-c 127.0.0.1:12500 \
-d 127.0.0.1:12501

$BUILD_DIR/msc2p1 \
-i 127.0.0.1:7000,127.0.0.1:7001 \
-o 127.0.0.1:5555 \
-m 127.0.0.1:5001 \
--ponythreads 1 \
--ponypinasio \
--ponynoblock \
-c 127.0.0.1:12500 \
-d 127.0.0.1:12501



#reset

