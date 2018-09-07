#!/bin/bash

#bash用法：
#   在用一sh进程中执行脚本script.sh:
#   source script.sh
#   . script.sh
#   注意这种用法，script.sh开头一行不能包含 #!/bin/bash

#   新建一个sh进程执行脚本script.sh:
#   sh script.sh
#   ./script.sh
#   注意这种用法，script.sh开头一行必须包含 #!/bin/bash

#作者：康林

#参数:
#    $1:编译目标
#    $2:源码的位置 
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

#产生修改前缀脚本
if [ ! -f ${RABBIT_BUILD_PREFIX}/change_prefix.sh ]; then
    cp -f change_prefix.sh ${RABBIT_BUILD_PREFIX}/change_prefix.sh
    sed -i.orig -e "s,@@CONTRIB_PREFIX@@,${RABBIT_BUILD_PREFIX},g" ${RABBIT_BUILD_PREFIX}/change_prefix.sh
    rm -f ${RABBIT_BUILD_PREFIX}/change_prefix.sh.orig
fi
