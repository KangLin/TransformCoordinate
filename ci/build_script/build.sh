#!/bin/bash
set -ev

if [ "$BUILD_TARGERT" = "windows_mingw" \
    -a -n "$APPVEYOR" ]; then
    export RABBIT_TOOLCHAIN_ROOT=/C/Qt/Tools/mingw${RABBIT_TOOLCHAIN_VERSION}_32
    export PATH="${RABBIT_TOOLCHAIN_ROOT}/bin:/usr/bin:/c/Tools/curl/bin:/c/Program Files (x86)/CMake/bin"
fi
TARGET_OS=`uname -s`
case $TARGET_OS in
    MINGW* | CYGWIN* | MSYS*)
        export PKG_CONFIG=/c/msys64/mingw32/bin/pkg-config.exe
        ;;
    Linux* | Unix*)
    ;;
    *)
    ;;
esac
if [ "$BUILD_TARGERT" = "windows_msvc" ]; then
    export PATH=/C/Perl/bin:$PATH
    rm -fr /usr/include
fi

PROJECT_DIR=`pwd`
if [ -n "$1" ]; then
    PROJECT_DIR=$1
fi
echo "PROJECT_DIR:${PROJECT_DIR}"
SCRIPT_DIR=${PROJECT_DIR}/ci/build_script

cd ${SCRIPT_DIR}
SOURCE_DIR=${PROJECT_DIR}/ci/src
TOOLS_DIR=${PROJECT_DIR}/Tools

export RABBIT_BUILD_PREFIX=${PROJECT_DIR}/install #${BUILD_TARGERT}${RABBIT_TOOLCHAIN_VERSION}_${RABBIT_ARCH}_qt${QT_VERSION}_${RABBIT_CONFIG}
if [ ! -d ${RABBIT_BUILD_PREFIX} ]; then
    mkdir -p ${RABBIT_BUILD_PREFIX}
fi
cd ${RABBIT_BUILD_PREFIX}
export RABBIT_BUILD_PREFIX=`pwd`
cd ${SCRIPT_DIR}
if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${TOOLS_DIR}/android-sdk
    export ANDROID_NDK_ROOT=${TOOLS_DIR}/android-ndk
    export RABBIT_TOOL_CHAIN_ROOT=${TOOLS_DIR}/android-ndk/android-toolchain-${RABBIT_ARCH}
    if [ -z "$APPVEYOR" ]; then
        export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    fi
    QT_DIR=${TOOLS_DIR}/Qt/Qt${QT_VERSION}/${QT_VERSION}
    case $RABBIT_ARCH in
        arm*)
            export QT_ROOT=${QT_DIR}/android_armv7
            ;;
        x86*)
            export QT_ROOT=${QT_DIR}/android_$RABBIT_ARCH
            ;;
           *)
           echo "Don't arch $RABBIT_ARCH"
           ;;
    esac
    export PATH=${TOOLS_DIR}/apache-ant/bin:$JAVA_HOME:$PATH
fi
if [ "$BUILD_TARGERT" != "windows_msvc" ]; then
    RABBIT_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
    if [ "$RABBIT_MAKE_JOB_PARA" = "-j1" ];then
            RABBIT_MAKE_JOB_PARA="-j2"
    fi
    export RABBIT_MAKE_JOB_PARA
fi

echo "PATH:$PATH"
./build_zlib.sh ${RABBIT_BUILD_TARGERT} ${SOURCE_DIR}/zlib
./build_minizip.sh ${RABBIT_BUILD_TARGERT} ${SOURCE_DIR}/minizip
./build_expat.sh ${RABBIT_BUILD_TARGERT} ${SOURCE_DIR}/expat
./build_boost.sh ${RABBIT_BUILD_TARGERT} ${SOURCE_DIR}/boost
./build_libcurl.sh ${RABBIT_BUILD_TARGERT} ${SOURCE_DIR}/libcurl
./build_libkml.sh ${RABBIT_BUILD_TARGERT} ${PROJECT_DIR}
