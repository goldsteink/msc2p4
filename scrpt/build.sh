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


# arg1: exec value
# arg2: should terminate
# arg3: Text
function printAndExit()
{
    if [ $# -lt 3 ] ; then
	echoRed "Not enough arguments for the function"
	return
    fi

    if [ $1 -gt 0 ] ; then
	echoRed "Unable to build: $3" >> $BUILD_LOG 2>&1
	echoRed "Unable to build: $3"
	if [ $2 -gt 0 ] ; then
	    exit 1
	fi
    else
	echoYellow "$3 Finished" >> $BUILD_LOG 2>&1
	echoYellow "$3 Finished"
    fi
}



# arg1: the file to clean
# arg2: show debug?
function cleanFile()
{
    if [ $# -lt 1 ] ; then
	echoRed "Not enough params for the function"
	return
    fi


    if [ -e $1 ] ; then
	if [ $2 -gt 0 ] ; then
	    echoBlue "   Cleaning: $1"
	fi
	rm $1
    fi
}



if [ $# -gt 0 ] ; then
    if [ "$1" == "-h" ] || [ "$1" == "?" ] || [ "$1" == "-?" ] ; then
	echoWhite "Parmas:";
	echoWhite "   <none>"
	echoWhite "   swig"
	echoWhite "   cpp"
	echoWhite "   pony"
	echoWhite "   link"
    fi
fi

CLEAN="no"
SWIG="no"
CPP="no"
PONY="no"
LINK="no"
if [ $# -eq 0 ] ; then
    echoBlue "Building whole project"
    SWIG="yes"
    CPP="yes"
    PONY="yes"
    LINK="yes"
    CLEAN="yes"
else
    for param in $@
    do
	if [[ "$param" == "swig" ]]; then
	    echoBlue "Only building SWIG"
	    SWIG="yes"
	elif [[ "$param" == "cpp" ]]; then
	    echoBlue "Only building CPP"
	    CPP="yes"
	elif [[ "$param" == "pony" ]]; then
	    echoBlue "Only building PONY"
	    PONY="yes"
	elif [[ "$param" == "link" ]]; then
	    echoBlue "Only running LINKER"
	    LINK="yes"
	elif [[ "$param" == "clean" ]]; then
	    echoBlue "Only cleaning project"
	    CLEAN="yes"
	fi
    done
fi

if [ ! -d $build_dir ] ; then
    mkdir -p $build_dir
fi

if [ "$CLEAN" == "yes" ] ; then
    showDebug=1
    cleanFile src/_MSPY.so $showDebug
    cleanFile src/Base_wrap.cpp $showDebug
    cleanFile src/Base_wrap.h $showDebug
    cleanFile src/market-spread-cpp.o $showDebug
    cleanFile src/msc2p4.o $showDebug
    cleanFile src/msc2p4 $showDebug
    cleanFile src/MSPY.py $showDebug
    cleanFile src/MSPY.pyc $showDebug
    cleanFile src/PyNbbo.pyc $showDebug
    cleanFile src/swigpyrun.h $showDebug
fi


if [ "$SWIG" == "yes" ] ; then
    echoBlue -n "Building SWIG..." > $BUILD_LOG 2>&1
    echoBlue "Building SWIG..."

    swig -python -external-runtime src/swigpyrun.h >> $BUILD_LOG 2>&1
    printAndExit $? 1 "SWIG external runtime"

    swig -shadow -cppext cpp -c++ -python src/Base.i >> $BUILD_LOG 2>&1
    printAndExit $? 1 "SWIG shadow classes"

    echoGreen "Injecting verbose errors" >> $BUILD_LOG 2>&1
    echoGreen "Injecting verbose errors"
    . ./scrpt/inject.sh
fi


if [ "$CPP" == "yes" ] ; then
    echoBlue "Building C++ Python Module... ">> $BUILD_LOG 2>&1
    echoBlue "Building C++ Python Module..."
    debugargs="-g3 -ggdb -O0"
    
    pushd src>> /dev/null 2>&1
    c++ -fPIC -fpermissive $debugargs -march=native -mcx16 -fuse-ld=gold \
	-shared -lpthread -ldl -lutil -lm -lpython2.7 -I/usr/include/python2.7 \
	-I"." \
	-I$WALL_INC_DIR -L$WALL_LIB_DIR -l$WALL_LIB_NAME \
	-DDEBUG \
	*.cpp -o _MSPY.so >> $BUILD_LOG 2>&1
    printAndExit $? 1 "C++ MSPY library"

    c++ -fPIC -fpermissive $debugargs -march=native -mcx16 -fuse-ld=gold \
	-shared -lpthread -ldl -lutil -lm -lpython2.7 -I/usr/include/python2.7 \
	-I"." \
	-I$WALL_INC_DIR -L$WALL_LIB_DIR -l$WALL_LIB_NAME \
	-DDEBUG \
	-c market-spread-cpp.cpp >> $BUILD_LOG 2>&1
    printAndExit $? 1 "C++ MSCPP Object"
    popd>>/dev/null 2>&1
fi


pony_obj=src/src.o
if [ "$PONY" == "yes" ] ; then
    echoBlue "Building pony" >> $BUILD_LOG 2>&1
    echoBlue "Building pony"

    if [ -e $pony_obj ] ; then
	echoRed "cleaning old object file: $pony_obj" >> $BUILD_LOG 2>&1
	rm $pony_obj
    fi
    if [ -e src/msc2p4.o ] ; then
	echoRed "cleaning old object file: src/msc2p4.o" >> $BUILD_LOG 2>&1
	rm src/msc2p4.o
    fi
    
    ponyc --path=$WALL_HOME:$WALL_LIB_DIR:$CPP_PONY:$C2P_LIB --output=src ./src >> $BUILD_LOG 2>&1
    if [ -e $pony_obj ] ; then
	printAndExit 0 1 "PONY compilation successfull"
	mv $pony_obj src/msc2p4.o
    else
	printAndExit 1 1 "PONY compilation failed"
    fi
fi


pushd ./src >> /dev/null 2>&1
if [ "$LINK" == "yes" ] ; then
    echoBlue "Pony/C++/Swig linking..." >> $BUILD_LOG 2>&1
    echoBlue "Pony/C++/Swig linking..."
    c++ -o $PRJSRC/msc2p4 -O0 -g -march=native -mcx16 -fuse-ld=gold $PRJSRC/*.o \
	/usr/local/lib/WallarooCppApi/libwallaroo.a \
	-L$WALL_HOME -Wl,-rpath,$WALL_HOME \
	-L$WALL_API_DIR -Wl,-rpath,$WALL_API_DIR \
	-L$PRJSRC -Wl,-rpath,$PRJSRC \
	-L/usr/local/lib/pony/0.9.0-e405a8d/bin/../lib \
	-Wl,-rpath,/usr/local/lib/pony/0.9.0-e405a8d/bin/../lib \
	-L/usr/local/lib/pony/0.9.0-e405a8d/bin/../packages \
	-Wl,-rpath,/usr/local/lib/pony/0.9.0-e405a8d/bin/../packages \
	-L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--start-group \
	-lwallaroo -lstdc++ -lrt -Wl,--end-group -lponyrt -lpthread -ldl -lm -lpython2.7 \
	-l:_MSPY.so >> $BUILD_LOG 2>&1
    printAndExit $? 1 "Pony/C++/Swig linking"
fi
popd >> /dev/null 2>&1
