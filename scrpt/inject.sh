#!/bin/bash

#
# inject PyError_Print() into Base_wrap.cpp
#
FILE=/home/kgoldstein/dev/c++/msc2p4/src/Base_wrap.cpp
KEY="Error detected"
PREPEND="PyError_Print();"


sed -i '/Error detected/iPyErr_Print();' $FILE
