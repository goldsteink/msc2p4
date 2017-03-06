#!/bin/bash

. ~/bin/colors.sh

build_dir=${PWD}/build
BUILD_LOG=$build_dir/build.log
WALL_HOME=/home/kgoldstein/dev/buffy/lib
WALL_API_DIR=/usr/local/lib/WallarooCppApi
WALL_API_LIB=wallaroo
WALL_INC_DIR=/usr/local/include/WallarooCppApi
C2P_PONY=/home/kgoldstein/dev/c++/msc2p4/src
PROJ=/home/kgoldstein/dev/c++/msc2p4
PRJSRC=$PROJ/src

if [ ! -d $build_dir ] ; then
    mkdir -p $build_dir
fi

eghoGreen "Building SWIG..." > $BUILD_LOG 2>&1
echoBlue "Building SWIG..."
swig -python -external-runtime src/swigpyrun.h >> $BUILD_LOG 2>&1
swig -shadow -cppext cpp -c++ -python src/Base.i >> $BUILD_LOG 2>&1


pony_obj=src/src.o
if [ -e $pony_obj ] ; then
    echoRed "cleaning old object file: $pony_obj" >> $BUILD_LOG 2>&1
    rm $pony_obj
fi
if [ -e src/msc2p4.o ] ; then
    echoRed "cleaning old object file: src/msc2p4.o" >> $BUILD_LOG 2>&1
    rm src/msc2p4.o
fi


echoGreen "Building pony" >> $BUILD_LOG 2>&1
echoGreen "Building pony"
ponyc --path=$WALL_HOME:$WALL_LIB_DIR:$CPP_PONY:$C2P_LIB --output=src ./src >> $BUILD_LOG 2>&1
if [ -e $pony_obj ] ; then
    mv $pony_obj src/msc2p4.o
fi


echoGreen Building Python Module... >> $BUILD_LOG 2>&1
pushd src
c++ -fPIC -fpermissive -O0 -march=native -mcx16 -fuse-ld=gold \
-shared -lpthread -ldl -lutil -lm -lpython2.7 -I/usr/include/python2.7 \
-I$WALL_INC_DIR -L$WALL_LIB_DIR -l$WALL_LIB_NAME \
*.cpp -o _MSPY.so >> $BUILD_LOG 2>&1
if [ $? -gt 0 ] ; then
    echoRed "Failed!" >> $BUILD_LOG 2>&1
    echoRed "FAILED!"
    exit 1
fi


echoGreen "Linking overall project..." >> $BUILD_LOG 2>&1
c++ -o $PRJSRC/msc2p4 -O0 -g -march=native -mcx16 -fuse-ld=gold $PRJSRC/*.o \
-L$WALL_HOME -Wl,-rpath,$WALL_HOME \
-L$WALL_API_DIR -Wl,-rpath,$WALL_API_DIR \
-L$PRJSRC -Wl,-rpath,$PRJSRC \
-L/usr/local/lib/pony/0.9.0-e405a8d/bin/../lib \
-Wl,-rpath,/usr/local/lib/pony/0.9.0-e405a8d/bin/../lib \
-L/usr/local/lib/pony/0.9.0-e405a8d/bin/../packages \
-Wl,-rpath,/usr/local/lib/pony/0.9.0-e405a8d/bin/../packages \
-L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--start-group \
-lwallaroo -lstdc++ -lrt -Wl,--end-group -lponyrt -lpthread -ldl -lm -lpython2.7 >> $BUILD_LOG 2>&1
if [ $? -gt 0 ] ; then
    echoRed "FAILED TO BUILD!"
    echoRed "FAILED TO BUILD!" >> $BUILD_LOG
else
    echoBlue "Finished!"
    echoBlue "Finished!" >> $BUILD_LOG

fi

popd
