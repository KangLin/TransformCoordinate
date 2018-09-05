#!/bin/bash 

SOURCE_DIR=`pwd`

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
