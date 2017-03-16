#!/bin/bash

#
# inject PyError_Print() into Base_wrap.cpp
#
source ~/bin/colors.sh
FILE=$PWD/src/Base_wrap.cpp
echoBlue "File to fix: $FILE"
KEY="Error detected"
PREPEND="PyError_Print();"



#sed -i '/Error detected/iPyErr_Print();' $FILE
sed -i '/::raise/iPyErr_Print();' $FILE
if [ $? -gt 0 ] ; then
    echoRed "Injection Failed"
else
    echoYellow "Injection succeeded"
fi
