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
    RABBIT_BUILD_SOURCE_CODE=${RABBIT_BUILD_PREFIX}/../src/minizip
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBIT_BUILD_SOURCE_CODE} ]; then
    VERSION=master #2.5.3
    if [ "TRUE" = "${RABBIT_USE_REPOSITORIES}" ]; then
        echo "git clone -q --branch=${VERSION} https://github.com/nmoinvaz/minizip.git ${RABBIT_BUILD_SOURCE_CODE}"
        git clone -q https://github.com/nmoinvaz/minizip.git ${RABBIT_BUILD_SOURCE_CODE}
        #echo "git clone -q https://github.com/KangLin/minizip.git ${RABBIT_BUILD_SOURCE_CODE}"
        #git clone -q https://github.com/KangLin/minizip.git ${RABBIT_BUILD_SOURCE_CODE}
        cd ${RABBIT_BUILD_SOURCE_CODE}
        if [ "$VERSION" != "master" ]; then
            git checkout -b $VERSION $VERSION
        fi
    else
        mkdir -p ${RABBIT_BUILD_SOURCE_CODE}
        cd ${RABBIT_BUILD_SOURCE_CODE}
        echo "wget -nv -c https://github.com/nmoinvaz/minizip/archive/${VERSION}.zip"
        wget -nv -c -O minizip.zip https://github.com/nmoinvaz/minizip/archive/${VERSION}.zip
        #echo "wget -nv -c -O minizip.zip https://github.com/KangLin/minizip/archive/${VERSION}.zip"
        #wget -nv -c -O minizip.zip https://github.com/KangLin/minizip/archive/${VERSION}.zip
        
        unzip -q minizip.zip
        mv minizip-${VERSION} ..
        rm -fr minizip.zip ${RABBIT_BUILD_SOURCE_CODE}
        cd ..
        mv minizip-${VERSION} ${RABBIT_BUILD_SOURCE_CODE}
    fi
fi

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
echo "RABBIT_BUILD_STATIC:$RABBIT_BUILD_STATIC"
echo ""

echo "configure ..."
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
        CONFIG_PARA="${CONFIG_PARA} --with-gnu-ld --enable-sse "
        ;;
    windows_msvc)
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
        exit 3
    ;;
esac
CMAKE_PARA="${CMAKE_PARA} -DZLIB_ROOT=$RABBIT_BUILD_PREFIX"
echo "cmake .. -DCMAKE_INSTALL_PREFIX=$RABBIT_BUILD_PREFIX -DZLIB_ROOT=\"$RABBIT_BUILD_PREFIX\" -G\"${RABBITIM_GENERATORS}\" ${CMAKE_PARA}"
if [ "${RABBIT_BUILD_TARGERT}" = "android" ]; then
    cmake .. \
        -DCMAKE_INSTALL_PREFIX="$RABBIT_BUILD_PREFIX" \
        -DCMAKE_BUILD_TYPE=${RABBIT_CONFIG} \
        -G"${RABBITIM_GENERATORS}" ${CMAKE_PARA} -DANDROID_ABI="${ANDROID_ABI}" 
else
    cmake .. \
        -DCMAKE_INSTALL_PREFIX="$RABBIT_BUILD_PREFIX" \
        -DCMAKE_BUILD_TYPE=${RABBIT_CONFIG} \
        -G"${RABBITIM_GENERATORS}" ${CMAKE_PARA} 
fi
cmake --build . --target install --config ${RABBIT_CONFIG} ${MAKE_PARA}
case ${RABBIT_BUILD_TARGERT} in
    android)
    ;;
    unix)
        ;;
    windows_msvc)
        MAKE_PARA=""
        cd $RABBIT_BUILD_PREFIX/lib
        if [ -f libminizip.lib ]; then
            cp libminizip.lib minizip.lib
        fi
        ;;
    windows_mingw)
        ;;
    *)
    ;;
esac
cd $CUR_DIR
