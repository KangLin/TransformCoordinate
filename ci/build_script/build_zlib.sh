#!/bin/bash

#作者：康林
#参数:
#    $1:编译目标(android、windows_msvc、windows_mingw、unix)
#    $2:源码的位置 

#运行本脚本前,先运行 build_$1_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBIT_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix)
#   RABBIT_BUILD_PREFIX=`pwd`/../${RABBIT_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBIT_BUILD_SOURCE_CODE    #源码目录
#   RABBIT_BUILD_CROSS_PREFIX   #交叉编译前缀
#   RABBIT_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

set -e
HELP_STRING="Usage $0 PLATFORM(android|windows_msvc|windows_mingw|unix) [SOURCE_CODE_ROOT_DIRECTORY]"

case $1 in
    android|windows_msvc|windows_mingw|unix)
    RABBIT_BUILD_TARGERT=$1
    ;;
    *)
    echo "${HELP_STRING}"
    exit 1
    ;;
esac
RABBIT_BUILD_SOURCE_CODE=$2

echo ". `pwd`/build_envsetup_${RABBIT_BUILD_TARGERT}.sh"
. `pwd`/build_envsetup_${RABBIT_BUILD_TARGERT}.sh

if [ -z "$RABBIT_BUILD_SOURCE_CODE" ]; then
    RABBIT_BUILD_SOURCE_CODE=${RABBIT_BUILD_PREFIX}/../src/zlib
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBIT_BUILD_SOURCE_CODE} ]; then
    VERSION=1.2.11
    if [ "TRUE" = "${RABBIT_USE_REPOSITORIES}" ]; then
        echo "git clone -q https://github.com/madler/zlib.git ${RABBIT_BUILD_SOURCE_CODE}"
        git clone -q https://github.com/madler/zlib.git ${RABBIT_BUILD_SOURCE_CODE}
        if [ "$VERSION" != "master" ]; then
            git checkout -b v${VERSION} v${VERSION}
        fi
    else
        mkdir -p ${RABBIT_BUILD_SOURCE_CODE}
        cd ${RABBIT_BUILD_SOURCE_CODE}
        echo "wget -q -c -nv https://github.com/madler/zlib/archive/v${VERSION}.zip"
        wget -q -c -nv https://github.com/madler/zlib/archive/v${VERSION}.zip
        unzip -q v${VERSION}.zip
        mv zlib-${VERSION} ..
        rm -fr *
        cd ..
        rm -fr ${RABBIT_BUILD_SOURCE_CODE}
        mv -f zlib-${VERSION} ${RABBIT_BUILD_SOURCE_CODE}
    fi
fi

echo ""
echo "RABBIT_BUILD_TARGERT:${RABBIT_BUILD_TARGERT}"
echo "RABBIT_BUILD_SOURCE_CODE:$RABBIT_BUILD_SOURCE_CODE"
echo "CUR_DIR:`pwd`"
echo "RABBIT_BUILD_PREFIX:$RABBIT_BUILD_PREFIX"
echo "RABBIT_BUILD_HOST:$RABBIT_BUILD_HOST"
echo "RABBIT_BUILD_CROSS_HOST:$RABBIT_BUILD_CROSS_HOST"
echo "RABBIT_BUILD_CROSS_PREFIX:$RABBIT_BUILD_CROSS_PREFIX"
echo "RABBIT_BUILD_CROSS_SYSROOT:$RABBIT_BUILD_CROSS_SYSROOT"
echo "RABBIT_BUILD_STATIC:$RABBIT_BUILD_STATIC"
echo ""

cd ${RABBIT_BUILD_SOURCE_CODE}
mkdir -p build_${RABBIT_BUILD_TARGERT}
cd build_${RABBIT_BUILD_TARGERT}
if [ "$RABBIT_CLEAN" = "TRUE" ]; then
    rm -fr *
fi

MAKE_PARA="-- ${RABBIT_MAKE_JOB_PARA}"
if [ "$RABBIT_BUILD_STATIC" = "static" ]; then
    CONFIG_PARA="--enable-static --disable-shared"
else
    CONFIG_PARA="--disable-static --enable-shared"
fi
case ${RABBIT_BUILD_TARGERT} in
    android)
        if [ -n "$RABBIT_CMAKE_MAKE_PROGRAM" ]; then
            CMAKE_PARA="${CMAKE_PARA} -DCMAKE_MAKE_PROGRAM=$RABBIT_CMAKE_MAKE_PROGRAM" 
        fi
        CMAKE_PARA="${CMAKE_PARA} -DCMAKE_TOOLCHAIN_FILE=$RABBIT_BUILD_PREFIX/../build_script/cmake/platforms/toolchain-android.cmake"
        CMAKE_PARA="${CMAKE_PARA} -DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}"
        #CMAKE_PARA="${CMAKE_PARA} -DANDROID_ABI=${ANDROID_ABI}"   
    ;;
    unix)
        #CONFIG_PARA="${CONFIG_PARA} --with-gnu-ld --enable-sse "
        ;;
    windows_msvc)
        #cd ${RABBIT_BUILD_SOURCE_CODE}
        #nmake -f win32/Makefile.msc clean
        #nmake -f win32/Makefile.msc LOC="-DASMV -DASMINF" \
        #    OBJA="inffas32.obj match686.obj"  
        #if [ ! -d "${RABBIT_BUILD_PREFIX}/include" ]; then
        #    mkdir -p "${RABBIT_BUILD_PREFIX}/include"
        #fi
        #if [ ! -d "${RABBIT_BUILD_PREFIX}/lib" ]; then
        #    mkdir -p "${RABBIT_BUILD_PREFIX}/lib"
        #fi
        #if [ ! -d "${RABBIT_BUILD_PREFIX}/bin" ]; then
        #    mkdir -p "${RABBIT_BUILD_PREFIX}/bin"
        #fi
        #cp zlib.h zconf.h ${RABBIT_BUILD_PREFIX}/include
        #cp *.lib ${RABBIT_BUILD_PREFIX}/lib
        #cp *.dll ${RABBIT_BUILD_PREFIX}/bin
        #cd $CUR_DIR
        #exit 0
        
        MAKE_PARA=""
        #CMAKE_PARA="${CMAKE_PARA}"  -DASM686=ON"
        ;;
    windows_mingw)
        #cd ${RABBIT_BUILD_SOURCE_CODE}
        #make -f win32/Makefile.gcc clean
        #cp contrib/asm686/match.S ./match.S
        #make LOC=-DASMV OBJA=match.o -fwin32/Makefile.gcc
        #make install -fwin32/Makefile.gcc SHARED_MODE=1 DESTDIR=${RABBIT_BUILD_PREFIX}/ \
        #     LIBRARY_PATH=lib \
        #     INCLUDE_PATH=include \
        #     BINARY_PATH=bin \
        #     prefix=${RABBIT_BUILD_PREFIX}
        #cd $CUR_DIR
        #exit 0
        
        #CMAKE_PARA="${CMAKE_PARA} -DASM686=ON"

        CMAKE_PARA="${CMAKE_PARA} -DCMAKE_TOOLCHAIN_FILE=$RABBIT_BUILD_PREFIX/../build_script/cmake/platforms/toolchain-mingw.cmake"
        ;;
    *)
    echo "${HELP_STRING}"
    cd $CUR_DIR
    exit 3
    ;;
esac

echo "cmake .. -DCMAKE_INSTALL_PREFIX=$RABBIT_BUILD_PREFIX -DCMAKE_BUILD_TYPE=${RABBIT_CONFIG} -G\"${RABBITIM_GENERATORS}\" ${CMAKE_PARA}"
if [ "${RABBIT_BUILD_TARGERT}" = "android" ]; then
    cmake .. \
        -DCMAKE_INSTALL_PREFIX="$RABBIT_BUILD_PREFIX" \
        -DCMAKE_BUILD_TYPE=${RABBIT_CONFIG} \
        -G"${RABBITIM_GENERATORS}" ${CMAKE_PARA} -DANDROID_ABI="${ANDROID_ABI}" 
else
    cmake .. \
        -DCMAKE_INSTALL_PREFIX="$RABBIT_BUILD_PREFIX" \
        -DCMAKE_BUILD_TYPE=${RABBIT_CONFIG} \
        -G"${RABBITIM_GENERATORS}" ${CMAKE_PARA} -DCMAKE_VERBOSE_MAKEFILE=TRUE 
fi
cmake --build . --target install --config ${RABBIT_CONFIG} ${MAKE_PARA}

mkdir -p $RABBIT_BUILD_PREFIX/lib/pkgconfig
cp $RABBIT_BUILD_PREFIX/share/pkgconfig/* $RABBIT_BUILD_PREFIX/lib/pkgconfig/.

cd $CUR_DIR
