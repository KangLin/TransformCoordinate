#注意：修改后的本文件不要上传代码库中

#bash用法：  
#   在用一sh进程中执行脚本script.sh:
#   source script.sh
#   . script.sh
#   注意这种用法，script.sh开头一行不能包含 #!/bin/sh
#   相当于包含关系

#   新建一个sh进程执行脚本script.sh:
#   sh script.sh
#   ./script.sh
#   注意这种用法，script.sh开头一行必须包含 #!/bin/sh  

if [ -z "${RABBIT_MAKE_JOB_PARA}" ]; then
    RABBIT_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
    if [ "$RABBIT_MAKE_JOB_PARA" = "-j1" ];then
        RABBIT_MAKE_JOB_PARA=
    fi
fi

#   RABBIT_BUILD_PREFIX=`pwd`/../${RABBIT_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBIT_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBIT_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot
if [ -z "${RABBIT_ARCH}" ]; then
    case $MSYSTEM in
        MINGW32)
            RABBIT_ARCH=x86
            ;;
        MINGW64)
            RABBIT_ARCH=x64
            ;;
        *)
            echo "Error RABBIT_ARCH=$MSYSTEM, set RABBIT_ARCH=x86"
            RABBIT_ARCH=x86
            ;;
    esac
    export RABBIT_ARCH=$RABBIT_ARCH
fi
case ${RABBIT_ARCH} in
    x86)
        if [ -z "${RABBIT_BUILD_CROSS_HOST}" ]; then
            RABBIT_BUILD_CROSS_HOST=i686-w64-mingw32 #编译工具链前缀
        fi
        ;;
    x64)
        if [ -z "${RABBIT_BUILD_CROSS_HOST}" ]; then
            RABBIT_BUILD_CROSS_HOST=x86_64-w64-mingw32 #编译工具链前缀
        fi
        ;;
    *)
        if [ -z "${RABBIT_BUILD_CROSS_HOST}" ]; then
            RABBIT_BUILD_CROSS_HOST=i686-w64-mingw32 #编译工具链前缀
        fi
        ;;
esac

export RABBIT_BUILD_CROSS_HOST=$RABBIT_BUILD_CROSS_HOST
RABBIT_BUILD_CROSS_PREFIX=${RABBIT_BUILD_CROSS_HOST}-

if [ -z "$RABBIT_BUILD_CROSS_SYSROOT" -a -n "${RABBIT_TOOLCHAIN_ROOT}" ];then
    export RABBIT_BUILD_CROSS_SYSROOT=${RABBIT_TOOLCHAIN_ROOT}/${RABBIT_BUILD_CROSS_HOST}
fi

if [ -z "$RABBIT_CONFIG" ]; then
    RABBIT_CONFIG=Release
fi

if [ -z "${RABBIT_BUILD_PREFIX}" ]; then
    RABBIT_BUILD_PREFIX=`pwd`/../${RABBIT_BUILD_TARGERT}    #修改这里为安装前缀  
    RABBIT_BUILD_PREFIX=${RABBIT_BUILD_PREFIX}${RABBIT_TOOLCHAIN_VERSION}_${RABBIT_ARCH}_qt${QT_VERSION}_${RABBIT_CONFIG}
    if [ "$RABBIT_BUILD_STATIC" = "static" ]; then
        RABBIT_BUILD_PREFIX=${RABBIT_BUILD_PREFIX}_static
    fi
fi
if [ ! -d ${RABBIT_BUILD_PREFIX} ]; then
    mkdir -p ${RABBIT_BUILD_PREFIX}
fi

MAKE="make ${RABBIT_MAKE_JOB_PARA}"
#自动判断主机类型，目前只做了linux、windows判断
TARGET_OS=`uname -s`
case $TARGET_OS in
    MINGW* | CYGWIN* | MSYS*)
        MAKE=make
        export PKG_CONFIG_PATH=${RABBIT_BUILD_PREFIX}/lib/pkgconfig:$PKG_CONFIG_PATH
        RABBITIM_GENERATORS="MSYS Makefiles"
        ;;
    Linux* | Unix*)
        #pkg-config帮助文档：http://linux.die.net/man/1/pkg-config
        export PKG_CONFIG_PATH=${RABBIT_BUILD_PREFIX}/lib/pkgconfig
        export PKG_CONFIG_LIBDIR=${PKG_CONFIG_PATH}
        export PKG_CONFIG_SYSROOT_DIR=${RABBIT_BUILD_PREFIX}
        RABBITIM_GENERATORS="Unix Makefiles" 
        ;;
    *)
    echo "Please set RABBIT_BUILD_HOST. see build_envsetup_windows_mingw.sh"
    return 2
    ;;
esac

if [ -z "$PKG_CONFIG" ]; then
    export PKG_CONFIG=pkg-config 
fi
if [ "$RABBIT_BUILD_STATIC" = "static" ]; then
    export PKG_CONFIG="${PKG_CONFIG} --static"
fi

if [ -z "$RABBIT_USE_REPOSITORIES" ]; then
    RABBIT_USE_REPOSITORIES="FALSE" #下载开发库。省略，则下载指定的压缩包
fi

export PKG_CONFIG_PATH=${RABBIT_BUILD_PREFIX}/lib/pkgconfig
export PKG_CONFIG_LIBDIR=${PKG_CONFIG_PATH}
if [ -n "$RABBIT_BUILD_CROSS_SYSROOT" ]; then
    export RABBIT_CFLAGS="--sysroot=${RABBIT_BUILD_CROSS_SYSROOT}" 
    export RABBIT_CPPFLAGS="--sysroot=${RABBIT_BUILD_CROSS_SYSROOT}" 
    export RABBIT_LDFLAGS="--sysroot=${RABBIT_BUILD_CROSS_SYSROOT}"
    
    RABBIT_BUILD_CROSS_STL=${RABBIT_BUILD_CROSS_SYSROOT}
    RABBIT_BUILD_CROSS_STL_INCLUDE=${RABBIT_BUILD_CROSS_STL}/include/c++
    #RABBIT_BUILD_CROSS_STL_LIBS=${RABBIT_BUILD_CROSS_STL}/libs
    RABBIT_BUILD_CROSS_STL_INCLUDE_FLAGS="-I${RABBIT_BUILD_CROSS_STL_INCLUDE}" # -I${RABBIT_BUILD_CROSS_STL_LIBS}/include"
    export RABBIT_CPPFLAGS="$RABBIT_CFLAGS $RABBIT_BUILD_CROSS_STL_INCLUDE_FLAGS"    
fi

echo "---------------------------------------------------------------------------"
echo "RABBIT_BUILD_PREFIX:$RABBIT_BUILD_PREFIX"
echo "QT_BIN:$QT_BIN"
echo "QT_ROOT:$QT_ROOT"
echo "PKG_CONFIG_PATH:$PKG_CONFIG_PATH"
echo "PKG_CONFIG_SYSROOT_DIR:$PKG_CONFIG_SYSROOT_DIR"
echo "RABBIT_BUILD_CROSS_HOST:$RABBIT_BUILD_CROSS_HOST"
echo "RABBIT_BUILD_CROSS_SYSROOT:$RABBIT_BUILD_CROSS_SYSROOT"
echo "RABBIT_TOOLCHAIN_ROOT:$RABBIT_TOOLCHAIN_ROOT"
echo "RABBIT_BUILD_CROSS_STL:$RABBIT_BUILD_CROSS_STL"
echo "RABBIT_BUILD_CROSS_STL_INCLUDE:$RABBIT_BUILD_CROSS_STL_INCLUDE"
echo "PATH:$PATH"
echo "---------------------------------------------------------------------------"
