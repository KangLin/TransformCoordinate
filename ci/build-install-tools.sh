#!/bin/bash 
#下载工具  

set -e

SOURCE_DIR="`pwd`"
echo $SOURCE_DIR
TOOLS_DIR=${SOURCE_DIR}/Tools
PKG_DIR=${SOURCE_DIR}/Packages
echo ${TOOLS_DIR}

export ANDROID_SDK_ROOT=${TOOLS_DIR}/android-sdk
export ANDROID_NDK_ROOT=${TOOLS_DIR}/android-ndk

if [ ! -d "${TOOLS_DIR}" ]; then
    mkdir ${TOOLS_DIR}
fi
if [ ! -d "${PKG_DIR}" ];then
    mkdir ${PKG_DIR}
fi

echo "QT_VERSION:${QT_VERSION}"
cd ${TOOLS_DIR}
# Qt qt安装参见：https://github.com/benlau/qtci  
if [ -n "${QT_VERSION}" ]; then
    QT_DIR=${TOOLS_DIR}/Qt/Qt${QT_VERSION}
    if [ ! -d "${QT_DIR}" ]; then
        wget -c --no-check-certificate -nv http://download.qt.io/official_releases/qt/${QT_VERSION_DIR}/${QT_VERSION}/qt-opensource-linux-x64-${QT_VERSION}.run
        bash ${SOURCE_DIR}/ci/qt-installer.sh qt-opensource-linux-x64-${QT_VERSION}.run ${QT_DIR}
        rm qt-opensource-linux-x64-${QT_VERSION}.run
    fi
fi
exit 0

cd ${TOOLS_DIR}

#下载android ndk  
if [ ! -d "`pwd`/android-ndk" ]; then
    wget -c -nv http://dl.google.com/android/ndk/android-ndk-r10e-linux-x86_64.bin
    chmod u+x android-ndk-r10e-linux-x86_64.bin
    ./android-ndk-r10e-linux-x86_64.bin > /dev/null
    mv android-ndk-r10e android-ndk
    rm android-ndk-r10e-linux-x86_64.bin
fi

cd ${TOOLS_DIR}

#Download android sdk  
if [ ! -d "`pwd`/android-sdk" ]; then
    wget -c -nv https://dl.google.com/android/android-sdk_r24.4.1-linux.tgz
    tar xf android-sdk_r24.4.1-linux.tgz 
    mv android-sdk-linux android-sdk
    rm android-sdk_r24.4.1-linux.tgz 
    (sleep 5 ; while true ; do sleep 1 ; printf 'y\r\n' ; done ) \
    | android-sdk/tools/android update sdk -u -t tool,android-18,android-24,extra,platform-tools,build-tools #platforms
fi

sudo apt-get install -y libkml-dev

cd ${SOURCE_DIR}
