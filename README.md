# 坐标系统转换

作者：康 林 <kl222@126.com>

[![Windows build status](https://ci.appveyor.com/api/projects/status/yxkcu6b6o2av6wmk?svg=true)](https://ci.appveyor.com/project/KangLin/transformcoordinate)
[![build](https://github.com/KangLin/TransformCoordinate/actions/workflows/build.yml/badge.svg)](https://github.com/KangLin/TransformCoordinate/actions/workflows/build.yml)
[![GitHub stars](https://img.shields.io/github/stars/KangLin/TransformCoordinate?label=点赞量)](https://star-history.com/#KangLin/TransformCoordinate&Date)
[![Gitee stars](https://gitee.com/kl222/TransformCoordinate/badge/star.svg?theme=dark)](https://gitee.com/kl222/TransformCoordinate/stargazers)

[![Github 发行版本](https://img.shields.io/github/release-pre/KangLin/TransformCoordinate?label=Github%20发行版本)](https://github.com/KangLin/TransformCoordinate/releases)
[![Github 最后发行版本](https://img.shields.io/github/release/KangLin/TransformCoordinate?label=Github%20最后发行版本)](https://github.com/KangLin/TransformCoordinate/releases)

[![Github 所有发行版本下载量](https://img.shields.io/github/downloads/KangLin/TransformCoordinate/total?label=Github%20下载总量)](http://gra.caldis.me/?user=KangLin&repo=TransformCoordinate)
[![从 sourceforge 下载量](https://img.shields.io/sourceforge/dt/TransformCoordinate.svg?label=Sourceforge%20下载总量)](https://sourceforge.net/projects/TransformCoordinate/files/latest/download)

[![Download from sourceforge](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/TransformCoordinate/files/latest/download)

+ 项目地址：https://github.com/KangLin/TransformCoordinate
+ 镜像地址：
  - https://gitlab.com/kl222/TransformCoordinate
  - https://sourceforge.net/projects/transformcoordinate/
  - https://gitee.com/kl222/TransformCoordinate

### 介绍
本项目对WGS84、GCJ02、百度坐标系之间进行转换。

- WGS84：
为一种大地坐标系，也是目前广泛使用的全球卫星定位系统（GPS）使用的坐标系。
- GCJ02：
戏称火星坐标系，是由中国国家测绘局制定的地理坐标系统，是由WGS84加密后得到的坐标系。
- BD09：
为百度坐标系，在GCJ02坐标系基础上再次加密。
其中bd09ll表示百度经纬度坐标，bd09mc表示百度墨卡托米制坐标。

本项目还包括一个GPX文件操作模块。

#### 本项目包含：

- C++ 库：
  - 坐标转换库：[TransformCoordinate](Src/TransformCoordinate.h)
  - GPX文件操作库：[GpxModel](Src/GpxModel/gpx_model.h)
- 坐标转换程序：TransformCoordinateApp

#### 支持平台

- [x] Windows 7 SP2 及以后
- [x] Linux
- [x] Android
- [x] MacOS
- [ ] IPHONE

**注意:** 本人没有 MacOS 和 IPHONE 设备。请有设备的朋友自行测试，并进行反馈。
也可以向本人[捐赠资金](#捐赠)或相关设备，请联系：<kl222@126.com>

详见：[Qt5 支持平台](https://doc.qt.io/qt-5/supported-platforms.html)、[Qt6 支持平台](https://doc.qt.io/qt-6/supported-platforms.html)。

### [下载安装包](https://github.com/KangLin/TransformCoordinate/releases/latest)

- Ubuntu
  - 包含玉兔公共库(RabbitCommon)
    - [开发库 libtransformcoordinate-dev_1.1.2_RabbitCommon_amd64.deb](https://github.com/KangLin/TransformCoordinate/releases/download/v1.1.2/libtransformcoordinate-dev_1.1.2_RabbitCommon_amd64.deb)
    - [运行库 libtransformcoordinate_1.1.2_RabbitCommon_amd64.deb](https://github.com/KangLin/TransformCoordinate/releases/download/v1.1.2/libtransformcoordinate_1.1.2_RabbitCommon_amd64.deb)
    - [转换示例程序 transformcoordinate_1.1.2_RabbitCommon_amd64.deb](https://github.com/KangLin/TransformCoordinate/releases/download/v1.1.2/transformcoordinate_1.1.2_RabbitCommon_amd64.deb)
    - 安装
    
          sudo apt install ./libtransformcoordinate_1.1.2_RabbitCommon_amd64.deb \
                           ./transformcoordinate_1.1.2_RabbitCommon_amd64.deb

  - 不包含玉兔公共库(RabbitCommon)。安装前需要先从 [RabbitCommon](https://github.com/KangLin/RabbitCommon/releases/latest) 下载，并安装 RabbitCommon。
    - [开发库 libtransformcoordinate-dev_1.1.2_amd64.deb](https://github.com/KangLin/TransformCoordinate/releases/download/v1.1.2/libtransformcoordinate-dev_1.1.2_amd64.deb)
    - [运行库 libtransformcoordinate_1.1.2_amd64.deb](https://github.com/KangLin/TransformCoordinate/releases/download/v1.1.2/libtransformcoordinate_1.1.2_amd64.deb)
    - [转换示例程序 transformcoordinate_1.1.2_amd64.deb](https://github.com/KangLin/TransformCoordinate/releases/download/v1.1.2/transformcoordinate_1.1.2_amd64.deb)
- Windows:
  - windows 7 及以后版本
    - [TransformCoordinate_v1.1.2_win64_msvc2019_64_qt6.8.1_Setup.exe](https://github.com/KangLin/TransformCoordinate/releases/download/v1.1.2/TransformCoordinate_v1.1.2_win64_msvc2019_64_qt6.8.1_Setup.exe)
    - [TransformCoordinate_v1.1.2_win32_msvc2017_qt5.12.12_Setup.exe](https://github.com/KangLin/TransformCoordinate/releases/download/v1.1.2/TransformCoordinate_v1.1.2_win32_msvc2017_qt5.12.12_Setup.exe)
  - windows xp
    - [TransformCoordinate_v1.1.2_windows_xp_Setup.exe](https://github.com/KangLin/TransformCoordinate/releases/download/v1.1.2/TransformCoordinate_v1.1.2_windows_xp_Setup.exe)
- Android: Android 9.0 及以后版本
  - [TransformCoordinate_1.1.2_android_arm64_v8a_qt6.8.1_Release.apk](https://github.com/KangLin/TransformCoordinate/releases/download/v1.1.2/TransformCoordinate_1.1.2_android_arm64_v8a_qt6.8.1_Release.apk)
  - [TransformCoordinate_1.1.2_android_x86_64_qt6.8.1_Release.apk](https://github.com/KangLin/TransformCoordinate/releases/download/v1.1.2/TransformCoordinate_1.1.2_android_x86_64_qt6.8.1_Release.apk)
- [更多的下载包](https://github.com/KangLin/TransformCoordinate/releases/latest)

### 编译
#### 下载源码

    git clone https://github.com/KangLin/TransformCoordinate.git

#### 依赖
+ 编译工具
  + [Qt](http://qt.io/)
  + 编译器
    - For linux or android
        + GNU Make 工具
        + GCC 或者 Clang 编译器
    - For windows
        + [MSVC](http://msdn.microsoft.com/zh-cn/vstudio)
        + MinGW
  + [CMake](http://www.cmake.org/)
+ 依赖库
  - [必选] Rabbit 公共库(RabbitCommon):
    - 源码： `https://github.com/KangLin/RabbitCommon`
    - 开发包： `https://github.com/KangLin/RabbitCommon/releases`
  
#### CMake 配置参数
  - [必选] QT_DIR: qt 安装位置
    - [可选] Qt5_DIR: qt5 安装位置
    - [可选] Qt6_DIR: qt6 安装位置
  - [可选] RabbitCommon_DIR: RabbitCommon 源码位置

#### 各平台编译
##### linux 平台编译说明

- 编译

      git clone https://github.com/KangLin/RabbitCommon.git
      git clone https://github.com/KangLin/TransformCoordinate.git
      cd TransformCoordinate
      mkdir build
      cd build
      cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install \
          -DCMAKE_BUILD_TYPE=Release \
          -DQT_DIR=...... \
          -DQt6_DIR=...... \
          -DRabbitCommon_DIR= \
          [其它可选 CMake 配置参数]
      cmake --build . --config Release

- 打包

      cmake --build . --config Release --target package

- 运行例子
  + [可选] 把生成库的目录加入到变量 LD_LIBRARY_PATH 中

        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/lib

  + 执行 bin 目录下的程序

        cd TransformCoordinate
        cd build
        cd bin
        ./TransformCoordinateApp

##### windows 平台编译说明
- 使用 cmake-gui.exe 工具编译。打开 cmake-gui.exe 配置
- 命令行编译
  + 把 cmake 命令所在目录加入到环境变量 PATH 中
  + 从开始菜单打开 “VS2015开发人员命令提示”，进入命令行

    - 编译

          cd TransformCoordinate
          mkdir build
          cd build
          cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install ^
              -DCMAKE_BUILD_TYPE=Release ^
              -DQT_DIR=...... ^
              -DQt6_DIR=...... ^
              -DRabbitCommon_DIR= ^
              [其它可选 CMake 配置参数]
          cmake --build . --config Release

    - 打包

          cmake --build . --config Release --target package

    - 运行例子
      + 执行 bin 目录下的程序
        - TransformCoordinateApp.exe

##### Android 平台编译说明

+ 安装 ndk 编译工具
  - 从 https://developer.android.com/ndk/downloads 下载 ndk，并安装到：/home/android-ndk
  - 设置环境变量：

        export ANDROID_NDK=/home/android-ndk
        
+ 安装 sdk 工具
  - 从 https://developer.android.google.cn/studio/releases 下载 sdk tools, 并安装到 /home/android-sdk
  - 设置环境变量：
  
        export ANDROID_SDK=/home/android-sdk

+ 编译
  - 主机是 linux

        cd TransformCoordinate
        mkdir build
        cd build
        cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/android-build \
                 -DCMAKE_BUILD_TYPE=Release \
                 -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
                 -DANDROID_ABI="armeabi-v7a with NEON" \
                 -DANDROID_PLATFORM=android-18 \
                 -DQT_DIR=...... \
                 -DQt6_DIR=...... \
                 -DRabbitCommon_DIR= \
                 [其它可选 CMake 配置参数]
        cmake --build . --config Release --target package

  - 主机是 windows

        cd TransformCoordinate
        mkdir build
        cd build
        cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/android-build ^
                 -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ^
                 -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake ^
                 -DCMAKE_MAKE_PROGRAM=${ANDROID_NDK}/prebuilt/windows-x86_64/bin/make.exe ^
                 -DANDROID_ABI=arm64-v8a ^
                 -DANDROID_ARM_NEON=ON ^
                 -DQT_DIR=...... ^
                 -DQt6_DIR=...... ^
                 -DRabbitCommon_DIR= ^
                 [其它可选 CMake 配置参数]
        cmake --build . --config Release --target package

  - CMake for android 参数说明：https://developer.android.google.cn/ndk/guides/cmake
    + ANDROID_ABI: 可取下列值：
      目标 ABI。如果未指定目标 ABI，则 CMake 默认使用 armeabi-v7a。  
      有效的目标名称为：
      - armeabi：带软件浮点运算并基于 ARMv5TE 的 CPU。
      - armeabi-v7a：带硬件 FPU 指令 (VFPv3_D16) 并基于 ARMv7 的设备。
      - armeabi-v7a with NEON：与 armeabi-v7a 相同，但启用 NEON 浮点指令。这相当于设置 -DANDROID_ABI=armeabi-v7a 和 -DANDROID_ARM_NEON=ON。
      - arm64-v8a：ARMv8 AArch64 指令集。
      - x86：IA-32 指令集。
      - x86_64 - 用于 x86-64 架构的指令集。
    + ANDROID_NDK \<path\> 主机上安装的 NDK 根目录的绝对路径
    + ANDROID_PLATFORM: 如需平台名称和对应 Android 系统映像的完整列表，请参阅 [Android NDK 原生 API](https://developer.android.google.cn/ndk/guides/stable_apis.html)
    + ANDROID_ARM_MODE
    + ANDROID_ARM_NEON
    + ANDROID_STL: 指定 CMake 应使用的 STL。默认情况下，CMake 使用 c++_static。 
      - c++_shared: 使用 libc++ 动态库
      - c++_static: 使用 libc++ 静态库
      - none: 没有 C++ 库支持
      - system: 用系统的 STL
  - 安装 apk 到设备

       adb install android-build-debug.apk 

##### MacOS 平台编译说明
- 编译

      cd TransformCoordinate
      mkdir build
      cd build
      cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install \
          -DCMAKE_BUILD_TYPE=Release \
          -DQT_DIR=...... \
          -DQt6_DIR=...... \
          -DRabbitCommon_DIR= \
          [其它可选 CMake 配置参数]
      cmake --build . --config Release

- 打包

      cmake --build . --config Release --target package

### 使用 C++ 库

- 坐标转换库:
  - 在程序 CMakeLists.txt 中加入下面行

        find_package(TransformCoordinate)
        target_link_libraries(${PROJECT_NAME} PRIVATE TransformCoordinate)

  - 接口详见： [TransformCoordinate](Src/TransformCoordinate.h)
   
- GPX文件操作库: 
  - 编译时，需要设置 CMake 参数: WITH_GPXMODEL=ON
  - 在程序 CMakeLists.txt 中加入下面行
  
        find_package(GpxModel)
        target_link_libraries(${PROJECT_NAME} PRIVATE GpxModel)

  - 接口详见: [GpxModel](Src/GpxModel/gpx_model.h)

### 脚本

- build_debpackage.sh
  + 此脚本是 linux 下生成 deb 包的。使用前，请确保安装了下面程序

        sudo apt-get install debhelper fakeroot

  + 用系统自带的 QT

        sudo apt-get install \
            qt6-tools-dev qt6-tools-dev-tools \
            qt6-base-dev qt6-base-dev-tools qt6-qpa-plugins \
            libqt6svg6-dev qt6-l10n-tools qt6-translations-l10n \
            qt6-scxml-dev qt6-multimedia-dev libqt6serialport6-dev \
            qt6-webengine-dev qt6-webengine-dev-tools

  + 详见: [ubuntu.yml](.github/workflows/ubuntu.yml)
  + 注意:
    - 如果使用 RabbitCommon 源码编译，则本库安装的库位于： /opt/TransformCoordinate
    - 如果使用 RabbitCommon 安装包编译，则本库安装于系统目录: /usr

- deploy.sh: 此脚本用于产生新的发行版本号和标签。仅程序发布者使用。

### 捐赠:

本软件如果对你有用，或者你喜欢它，请你捐赠，支持作者。谢谢！

[![捐赠](https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠")](https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠")
