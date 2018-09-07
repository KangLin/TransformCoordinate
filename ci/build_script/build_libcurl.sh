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
    RABBIT_BUILD_SOURCE_CODE=${RABBIT_BUILD_PREFIX}/../src/libcurl
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBIT_BUILD_SOURCE_CODE} ]; then
	CURL_FILE=curl-7_61_1
    if [ "TRUE" = "${RABBIT_USE_REPOSITORIES}" ]; then
        echo "git clone -q  git://github.com/bagder/curl.git ${RABBIT_BUILD_SOURCE_CODE}"
        #git clone -q --branch=$CURL_FILE git://github.com/bagder/curl.git ${RABBIT_BUILD_SOURCE_CODE}
        git clone -q --branch=$CURL_FILE git://github.com/bagder/curl.git ${RABBIT_BUILD_SOURCE_CODE}
    else
        echo "wget  -q https://github.com/bagder/curl/archive/${CURL_FILE}.zip"
        mkdir -p ${RABBIT_BUILD_SOURCE_CODE}
        cd ${RABBIT_BUILD_SOURCE_CODE}
        wget -c -q https://github.com/bagder/curl/archive/${CURL_FILE}.zip
        unzip -q  ${CURL_FILE}.zip
        mv curl-${CURL_FILE} ..
        rm -fr *
        cd ..
        rm -fr ${RABBIT_BUILD_SOURCE_CODE}
        mv -f curl-${CURL_FILE} ${RABBIT_BUILD_SOURCE_CODE}
    fi
fi

cd ${RABBIT_BUILD_SOURCE_CODE}

if [ "$RABBIT_CLEAN" = "TRUE" ]; then
    if [ -d ".git" ]; then
        echo "git clean -xdf"
        git clean -xdf
        git reset --hard HEAD
    else
        if [ "${RABBIT_BUILD_TARGERT}" != "windows_msvc" -a -f Makefile ]; then
            make distclean
        fi
    fi
fi

if [ ! -f configure ]; then
    echo "sh buildconf"
    if [ "${RABBIT_BUILD_TARGERT}" != "windows_msvc" ]; then
        ./buildconf
    fi
fi

if [ "${RABBIT_BUILD_TARGERT}" = "windows_msvc" ]; then
    if [ -n "$RABBIT_CLEAN" ]; then
        rm -fr builds
    fi
else
    mkdir -p build_${RABBIT_BUILD_TARGERT}
    cd build_${RABBIT_BUILD_TARGERT}
    if [ -n "$RABBIT_CLEAN" ]; then
        rm -fr *
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

echo "configure ..."
if [ "$RABBIT_BUILD_STATIC" = "static" ]; then
    CONFIG_PARA="--enable-static --disable-shared"
else
    CONFIG_PARA="--disable-static --enable-shared"
fi

case ${RABBIT_BUILD_TARGERT} in
    windows_mingw)
        #export CC=${RABBIT_BUILD_CROSS_PREFIX}gcc
        #export CXX=${RABBIT_BUILD_CROSS_PREFIX}g++
        #export AR=${RABBIT_BUILD_CROSS_PREFIX}gcc-ar
        #export LD=${RABBIT_BUILD_CROSS_PREFIX}ld
        #export AS=${RABBIT_BUILD_CROSS_PREFIX}as
        #export STRIP=${RABBIT_BUILD_CROSS_PREFIX}strip
        #export NM=${RABBIT_BUILD_CROSS_PREFIX}nm
        #CONFIG_PARA="CC=${RABBIT_BUILD_CROSS_PREFIX}gcc LD=${RABBIT_BUILD_CROSS_PREFIX}ld"
        CONFIG_PARA="--enable-static --disable-shared"
        CONFIG_PARA="${CONFIG_PARA} --host=$RABBIT_BUILD_CROSS_HOST --target=$RABBIT_BUILD_CROSS_HOST"
        #CONFIG_PARA="${CONFIG_PARA} --with-sysroot=${RABBIT_BUILD_CROSS_SYSROOT}"
        CONFIG_PARA="${CONFIG_PARA} --with-gnu-ld "
        CFLAGS="${RABBIT_CFLAGS}"
        CPPFLAGS="${RABBIT_CPPFLAGS}"
        ;;
    android) 
        export CC=${RABBIT_BUILD_CROSS_PREFIX}gcc 
        export CXX=${RABBIT_BUILD_CROSS_PREFIX}g++
        export AR=${RABBIT_BUILD_CROSS_PREFIX}gcc-ar
        export LD=${RABBIT_BUILD_CROSS_PREFIX}ld
        export AS=${RABBIT_BUILD_CROSS_PREFIX}as
        export STRIP=${RABBIT_BUILD_CROSS_PREFIX}strip
        export NM=${RABBIT_BUILD_CROSS_PREFIX}nm
        CONFIG_PARA="${CONFIG_PARA} CC=${RABBIT_BUILD_CROSS_PREFIX}gcc LD=${RABBIT_BUILD_CROSS_PREFIX}ld"
        CONFIG_PARA="${CONFIG_PARA} --host=$RABBIT_BUILD_CROSS_HOST --target=$RABBIT_BUILD_CROSS_HOST"
        #CONFIG_PARA="${CONFIG_PARA} --with-sysroot=${RABBIT_BUILD_CROSS_SYSROOT}"
        CONFIG_PARA="${CONFIG_PARA} --with-gnu-ld "
        CFLAGS="${RABBIT_CFLAGS}"
        CPPFLAGS="${RABBIT_CPPFLAGS}"
        LDFLAGS="${RABBIT_LDFLAGS}"
        ;;
    unix)
        CONFIG_PARA="${CONFIG_PARA} --with-gnu-ld --enable-sse"
        ;;
    windows_msvc)
        #cmake .. -G"Visual Studio 12 2013" \
        #    -DCMAKE_INSTALL_PREFIX="$RABBIT_BUILD_PREFIX" \
        #    -DCMAKE_BUILD_TYPE="Release" \
        #    -DBUILD_CURL_TESTS=OFF \
        #    -DCURL_STATICLIB=OFF
        #;;
        cd ${RABBIT_BUILD_SOURCE_CODE}
        ./buildconf.bat
        cd winbuild
        if [ "$RABBIT_BUILD_STATIC" = "static" ]; then
            MODE=static
        else
            MODE=dll
        fi
        if [ "$Debug" = "$RABBIT_CONFIG" ]; then
            DEBUG=yes
        else
            DEBUG=no
        fi
        nmake -f Makefile.vc mode=$MODE VC=${VC_TOOLCHAIN} WITH_DEVEL=$RABBIT_BUILD_PREFIX \
            MACHINE=$RABBIT_ARCH MACHINE=${RABBIT_ARCH} DEBUG=${DEBUG} 
        cp -fr ${RABBIT_BUILD_SOURCE_CODE}/builds/libcurl-vc${VC_TOOLCHAIN}-$RABBIT_ARCH-release-${MODE}-ipv6-sspi-winssl/* ${RABBIT_BUILD_PREFIX}
        cd $CUR_DIR
        exit 0
        ;;
    *)
        echo "${HELP_STRING}"
        cd $CUR_DIR
        exit 3
        ;;
esac

echo "make install"
echo "pwd:`pwd`"
CONFIG_PARA="${CONFIG_PARA} --disable-manual --enable-verbose"
#CONFIG_PARA="${CONFIG_PARA} --enable-libgcc  "
CONFIG_PARA="${CONFIG_PARA} --with-ssl=${RABBIT_BUILD_PREFIX} --with-zlib=${RABBIT_BUILD_PREFIX}"
#CONFIG_PARA="${CONFIG_PARA} --with-gnutls=${RABBIT_BUILD_PREFIX} --with-nss=${RABBIT_BUILD_PREFIX}"
#CONFIG_PARA="${CONFIG_PARA} --with-libssh2=${RABBIT_BUILD_PREFIX} --with-libidn=${RABBIT_BUILD_PREFIX}"
#CONFIG_PARA="${CONFIG_PARA} --with-winidn=${RABBIT_BUILD_PREFIX} --with-libidn=${RABBIT_BUILD_PREFIX}"
#CONFIG_PARA="${CONFIG_PARA} --with-nghttp2=${RABBIT_BUILD_PREFIX} --with-librtmp=${RABBIT_BUILD_PREFIX}"  #--with-sysroot=${RABBIT_BUILD_PREFIX}"
CONFIG_PARA="${CONFIG_PARA} --prefix=$RABBIT_BUILD_PREFIX"
echo "../configure ${CONFIG_PARA} CFLAGS=\"${CFLAGS=}\" CPPFLAGS=\"${CPPFLAGS}\" CXXFLAGS=\"${CPPFLAGS}\" LDFLAGS=\"${LDFLAGS}\""
../configure ${CONFIG_PARA} \
    CFLAGS="${CFLAGS}" \
    CPPFLAGS="${CPPFLAGS}" \
    CXXFLAGS="${CPPFLAGS}" \
    LDFLAGS="${LDFLAGS}"

${MAKE} V=1 
${MAKE} install

cd $CUR_DIR
