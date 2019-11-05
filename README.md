## 坐标系统转


作者：康林(kl222@126.com)  
项目地址：https://github.com/KangLin/TransformCoordinate  

------------
[![Windows build status](https://ci.appveyor.com/api/projects/status/yxkcu6b6o2av6wmk?svg=true)](https://ci.appveyor.com/project/KangLin/transformcoordinate)  
[![Linux build Status](https://travis-ci.org/KangLin/TransformCoordinate.svg?branch=master)](https://travis-ci.org/KangLin/TransformCoordinate)

------------

### 介绍
本项目对WGS84、GCJ02、百度坐标系之间进行转换。

WGS84：为一种大地坐标系，也是目前广泛使用的GPS全球卫星定位系统使用的坐标系。  
GCJ02：又称火星坐标系，是由中国国家测绘局制定的地理坐标系统，是由WGS84加密后得到的坐标系。  
BD09：为百度坐标系，在GCJ02坐标系基础上再次加密。其中bd09ll表示百度经纬度坐标，bd09mc表示百度墨卡托米制坐标。  

本项目还包括一个GPX文件操作模块。

本项目包含：  
- 坐标转换库：TransformCoordinate  
- GPX文件操作库：GpxModel  
- 坐标转换程序：TransformCoordinateApp  

------------

### [下载安装包](https://github.com/KangLin/TransformCoordinate /releases/latest)

- linux
    - [TransformCoordinate_v0.2.6.tar.gz](https://github.com/KangLin/TransformCoordinate/releases/download/v0.2.6/TransformCoordinate_v0.2.6.tar.gz)  
      AppImage格式的执行程序，可直接运行在linux系统，详见：https://appimage.org/  
      使用:    
      1. 解压。复制TransformCoordinate_v0.2.6.tar.gz到安装目录，然后解压：

                mkdir TransformCoordinate
                cd TransformCoordinate
                cp $DOWNLOAD/TransformCoordinate_v0.2.6.tar.gz .
                tar xvfz TransformCoordinate_v0.2.6.tar.gz

      2. 安装
        
                ./install.sh install
        
      3. 如果需要，卸载
        
                ./install.sh remove

- ubuntu
    - [TransformCoordinate_0.2.6_amd64.deb](https://github.com/KangLin/TransformCoordinate/releases/download/v0.2.6/TransformCoordinate_0.2.6_amd64.deb)  
  deb 安装包,可用于　Ubuntu
  
- windows
    - [TransformCoordinate-Setup-v0.2.6.exe](https://github.com/KangLin/TransformCoordinate/releases/download/v0.2.6/TransformCoordinate-Setup-v0.2.6.exe)  
  Windows安装包，支持 Windows xp 以上系统 

- android
    + [android-build-debug.apk](https://github.com/KangLin/TransformCoordinate/releases/download/v0.2.6/android-build-debug.apk)

------------

### 编译：

    mkdir build
    cd build
    cmake ..
    cmake --build . 

------------

### 生成版本标签：
./tag.sh vX.X.X

------------

### 捐赠:  
本软件如果对你有用，或者你喜欢它，请你捐赠，支持作者。谢谢！

![捐赠](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠")

------------

### 参见：  
http://lbsyun.baidu.com/index.php?title=TransformCoordinate  
http://www.360doc.com/content/16/0721/16/9200790_577327509.shtml
