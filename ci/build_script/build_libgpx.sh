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

echo ". `pwd`/build_envsetup_${RABBIT_BUILD_TARGERT}.sh"
. `pwd`/build_envsetup_${RABBIT_BUILD_TARGERT}.sh

RABBIT_BUILD_SOURCE_CODE=$2
if [ -z "$RABBIT_BUILD_SOURCE_CODE" ]; then
    RABBIT_BUILD_SOURCE_CODE=${RABBIT_BUILD_PREFIX}/../src/libgpx
fi

#下载源码:
if [ ! -d ${RABBIT_BUILD_SOURCE_CODE} ]; then
    echo "git clone -q https://github.com/KangLin/gpxlib.git ${RABBIT_BUILD_SOURCE_CODE}"
    git clone -q https://github.com/KangLin/gpxlib.git ${RABBIT_BUILD_SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${RABBIT_BUILD_SOURCE_CODE}

mkdir -p build_${RABBIT_BUILD_TARGERT}
cd build_${RABBIT_BUILD_TARGERT}
if [ "$RABBIT_CLEAN" = "TRUE" ]; then
    rm -fr *
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
echo "RABBIT_MAKE_JOB_PARA:$RABBIT_MAKE_JOB_PARA"
echo "RABBIT_CMAKE_MAKE_PROGRAM:$RABBIT_CMAKE_MAKE_PROGRAM"
echo ""

#需要设置 CMAKE_MAKE_PROGRAM 为 make 程序路径。

MAKE_PARA="-- ${RABBIT_MAKE_JOB_PARA}"
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
    ;;
    windows_msvc)
        #RABBITIM_GENERATORS="Visual Studio 12 2013"
        MAKE_PARA=""
        ;;
    windows_mingw)
        case `uname -s` in
            Linux*|Unix*|CYGWIN*)
                CMAKE_PARA="${CMAKE_PARA} -DCMAKE_TOOLCHAIN_FILE=$RABBIT_BUILD_PREFIX/../build_script/cmake/platforms/toolchain-mingw.cmake"
                ;;
            *)
            ;;
        esac
        ;;
    *)
    echo "${HELP_STRING}"
    cd $CUR_DIR
    return 2
    ;;
esac

CMAKE_PARA="${CMAKE_PARA} -DCMAKE_VERBOSE_MAKEFILE=ON"
echo "cmake .. -DCMAKE_INSTALL_PREFIX=$RABBIT_BUILD_PREFIX -DCMAKE_BUILD_TYPE=Release -G\"${RABBITIM_GENERATORS}\" ${CMAKE_PARA}"
cmake .. \
    -DCMAKE_INSTALL_PREFIX="$RABBIT_BUILD_PREFIX" \
    -G"${RABBITIM_GENERATORS}" ${CMAKE_PARA} 

cmake --build . --target install --config ${RABBIT_CONFIG} ${MAKE_PARA}

cd $CUR_DIR
