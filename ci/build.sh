#!/bin/bash 
set -ev

SOURCE_DIR=`pwd`
export PATH=/usr/bin:$PATH
echo "PATH:$PATH"
if [ "$BUILD_TARGERT" = "windows_mingw" \
    -a -n "$APPVEYOR" ]; then
    export RABBIT_TOOLCHAIN_ROOT=/C/Qt/Tools/mingw${RABBIT_TOOLCHAIN_VERSION}_32
    export PATH="${RABBIT_TOOLCHAIN_ROOT}/bin:/usr/bin:/c/Tools/curl/bin:/c/Program Files (x86)/CMake/bin"
fi

if [ "$BUILD_TARGERT" = "windows_msvc" ]; then
    export PATH=/C/Perl/bin:$PATH
    rm -fr /usr/include
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
if [ -n "${QT_VERSION}" ]; then
    CMAKE_PARA="-DQt5_DIR=${SOURCE_DIR}/Tools/Qt/Qt${QT_VERSION}/lib/cmake/Qt5" 
else
    CMAKE_PARA="-DBUILD_APP=OFF"
fi

cd ${SOURCE_DIR}
mkdir build
cd build
cmake .. ${CMAKE_PARA}
cmake --build . 
