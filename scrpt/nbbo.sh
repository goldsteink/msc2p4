#!/bin/bash

m=10
if [ "$1" != "" ] ; then
    m=$1
fi
DATAFILE=/home/kgoldstein/dev/buffy/demos/marketspread/350k-nbbo-fixish.msg
echo "Using: $DATAFILE"
/home/kgoldstein/dev/buffy/giles/sender/sender \
-b 127.0.0.1:7001 \
-m $m \
-f $DATAFILE \
-r \
--ponythreads=1 \
-y \
-g 46 \
--ponypinasio \
--ponynoblock

