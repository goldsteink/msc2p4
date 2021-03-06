#!/bin/bash

. ~/bin/colors.sh

base_dir=`pwd`
echoBlue "Project located in: $base_dir"

build_dir=$base_dir/build
BUILD_LOG=$build_dir/build.log
WALL_HOME=/home/kgoldstein/dev/BleedingEdge/buffy/lib
WALL_API_DIR=/usr/local/lib/WallarooCppApi
WALL_API_LIB=wallaroo
WALL_INC_DIR=/usr/local/include/WallarooCppApi
C2P_PONY=$base_dir/src
PROJ=$base_dir
PRJSRC=$PROJ/src

YES="yes"
NO="no"

echoBlue "Project source/binary dir: $PRJSRC"


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
	echoWhite "   <none>|all"
	echoWhite "   swig"
	echoWhite "   cpp"
	echoWhite "   pony"
	echoWhite "   link"
    fi
fi

CLEAN=$NO
SWIG=$NO
CPP=$NO
PONY=$NO
LINK=$NO
NLOG=$NO
if [ $# -eq 0 ] ; then
    echoBlue "Building whole project"
    SWIG=$YES
    CPP=$YES
    PONY=$YES
    LINK=$YES
    CLEAN=$NO
else
    for param in $@
    do
	pval="${param,,}"
	if [[ "$pval" == "swig" ]]; then
	    echoBlue "Only building SWIG"
	    SWIG=$YES
	elif [[ "$pval" == "cpp" ]]; then
	    echoBlue "Only building CPP"
	    CPP=$YES
	elif [[ "$pval" == "pony" ]]; then
	    echoBlue "Only building PONY"
	    PONY=$YES
	elif [[ "$pval" == "link" ]]; then
	    echoBlue "Only running LINKER"
	    LINK=$YES
	elif [[ "$pval" == "clean" ]]; then
	    echoBlue "Only cleaning project"
	    CLEAN=$YES
	elif [[ "$pval" == "nlog" ]]; then
	    echoBlue "Cleaning log"
	    NLOG=$YES
	elif [[ "$pval" == "all" ]] ; then
	    CLEAN=$YES
	    LINK=$YES
	    PONY=$YES
	    CPP=$YES
	    SWIG=$YES
	    NLOG=$NO
	fi
    done
fi



if [ ! -d $build_dir ] ; then
    mkdir -p $build_dir
fi

if [ $CLEAN == $YES ] ; then
    showDebug=1
    cleanFile src/_MSPY.so $showDebug
    cleanFile src/market-spread-cpp.o $showDebug
    cleanFile src/msc2p4.o $showDebug
    cleanFile src/msc2p4 $showDebug
    cleanFile src/MSPY.pyc $showDebug
    cleanFile src/PyNbbo.pyc $showDebug
fi



if [ $NLOG == $YES ] ; then
    echoBlue "Cleaning BUILD_LOG"
    cleanFile $BUILD_LOG 1
fi


if [ $SWIG == $YES ] ; then
    echoBlue -n "Building SWIG..." > $BUILD_LOG 2>&1
    echoBlue "Building SWIG..."

    OSNAME=`uname`
    if [ "$OSNAME" != "Linux" ] ; then
	echoRed "SWIG generation only available on Linux"
    else
	swig -python -external-runtime src/swigpyrun.h >> $BUILD_LOG 2>&1
	printAndExit $? 1 "SWIG external runtime"
	
	swig -shadow -cppext cpp -c++ -python src/Base.i >> $BUILD_LOG 2>&1
	printAndExit $? 1 "SWIG shadow classes"
	
	echoGreen "Injecting verbose errors" >> $BUILD_LOG 2>&1
	echoGreen "Injecting verbose errors"
	. ./scrpt/inject.sh
    fi
fi


if [ $CPP == $YES ] ; then
    CPPCMD=`command -v c++`
    if [ "$CPPCMD" == "" ] ; then
	echoRed "C++ compiler not found"
	exit 1
    fi
    echoBlue "Building C++ Python Module... ">> $BUILD_LOG 2>&1
    echoBlue "Building C++ Python Module..."
    debugargs="-g3 -ggdb -O0"
    PYINC=`python-config --includes`
    PYLIBS=`python-config --ldflags`
    echoBlue "PyIncludes:$PYINC" >> $BUILD_LOG 2>&1
    echoBlue "PyLibs:$PYLIBS" >> $BUILD_LOG 2>&1
    
    OSNAME=`uname`
    echoGreen $OSNAME
    OSLD="-fuse-ld=gold"
    if [ "$OSNAME" == "Darwin" ] ; then
	OSLD="-ldl"
    fi

    COMPILER=`command -v c++`
    if [ "$COMPILER" == "" ] ; then
	echoRed "Can't find compiler!, looked with: 'command -v c++'"
	exit 1
    fi

    pushd $PRJSRC>> /dev/null 2>&1
    $COMPILER -fPIC -fpermissive $debugargs -march=native -mcx16 \
	-shared \
	$OSLD \
	$PYLIBS \
	$PYINC \
	-std=c++11 \
	-DDEBUG \
	-I"." \
	-I$WALL_INC_DIR -L$WALL_LIB_DIR -l$WALL_LIB_NAME \
	*.cpp -o _MSPY.so >> $BUILD_LOG 2>&1
    printAndExit $? 1 "C++ MSPY library"

    $COMPILER -fPIC -fpermissive $debugargs -march=native -mcx16 \
	-shared \
	$OSLD \
	$PYLIBS \
	$PYINC \
	-std=c++11 \
	-DDEBUG \
	-I"." \
	-I$WALL_INC_DIR -L$WALL_LIB_DIR -l$WALL_LIB_NAME \
	-c market-spread-cpp.cpp >> $BUILD_LOG 2>&1
    printAndExit $? 1 "C++ MSCPP Object"
    popd>>/dev/null 2>&1
fi


if [ $PONY == $YES ] ; then
    echoBlue "Building pony" >> $BUILD_LOG 2>&1
    echoBlue "Building pony"
    pony_obj=src/src.o
    OBJFILE=src/msc2p4.o
    if [ -e $pony_obj ] ; then
	echoRed "cleaning old object file: $pony_obj" >> $BUILD_LOG 2>&1
	rm $pony_obj
    fi
    if [ -e $OBJFILE ] ; then
	echoRed "cleaning old object file: src/msc2p4.o" >> $BUILD_LOG 2>&1
	rm $OBJFILE
    fi
    
    ponyc --path=$WALL_HOME:$WALL_LIB_DIR:$CPP_PONY:$C2P_LIB --output=src ./src >> $BUILD_LOG 2>&1
    if [ -e $pony_obj ] ; then
	printAndExit 0 1 "PONY compilation successfull"
	mv $pony_obj $OBJFILE
    else
	printAndExit 1 1 "PONY compilation failed"
    fi
fi


pushd ./src >> /dev/null 2>&1
if [ $LINK == $YES ] ; then
    echoBlue "Pony/C++/Swig linking..." >> $BUILD_LOG 2>&1
    echoBlue "Pony/C++/Swig linking..."
    BINFILE=$PRJSRC/msc2p4 

    OSLD="-fuse-ld=gold"
    OSNAME=`uname`
    if [ "$OSNAME" == "Darwin" ] ; then
	OSLD="-ldl"
    fi


    COMPILER=`command -v c++`
    if [ "$CPP" == "" ] ; then
	echoRed "Can't find compiler!, looked with: 'command -v c++'"
	return 1
    fi

    $COMPILER -o $BINFILE -O0 -g -march=native -mcx16 \
	$OSLD \
	$PRJSRC/*.o \
	$WALL_API_DIR/libwallaroo.a \
	-L$WALL_HOME -Wl,-rpath,$WALL_HOME \
	-L$WALL_API_DIR -Wl,-rpath,$WALL_API_DIR \
	-L$PRJSRC -Wl,-rpath,$PRJSRC \
	-L/usr/local/lib/pony/0.9.0-e405a8d/bin/../lib \
	-Wl,-rpath,/usr/local/lib/pony/0.9.0-e405a8d/bin/../lib \
	-L/usr/local/lib/pony/0.9.0-e405a8d/bin/../packages \
	-Wl,-rpath,/usr/local/lib/pony/0.9.0-e405a8d/bin/../packages \
	-L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--start-group \
	-lwallaroo \
	-lssl \
	-lcrypto \
	-lstdc++ -lrt -Wl,--end-group -lponyrt -lpthread -ldl -lm -lpython2.7 \
	-l:_MSPY.so >> $BUILD_LOG 2>&1
    printAndExit $? 1 "Linking complete! - [Output:$BINFILE] - "
fi
popd >> /dev/null 2>&1
