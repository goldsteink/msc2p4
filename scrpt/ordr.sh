#!/bin/bash



DATAFILE=/home/kgoldstein/dev/buffy/demos/marketspread/350k-orders-fixish.msg
echo "Using: $DATAFILE"

/home/kgoldstein/dev/buffy/giles/sender/sender \
-b 127.0.0.1:7000 \
-m 10 \
-f $DATAFILE \
-r \
--ponythreads=1 \
-y \
-g 46 \
--ponypinasio \
--ponynoblock


#-s 300 -i 2_500_000 \