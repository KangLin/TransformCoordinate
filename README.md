坐标系统转换

[![Build status](https://ci.appveyor.com/api/projects/status/yxkcu6b6o2av6wmk?svg=true)](https://ci.appveyor.com/project/KangLin/transformcoordinate)

本项目对WGS84、GCJ02、百度坐标系之间进行转换。

WGS84：为一种大地坐标系，也是目前广泛使用的GPS全球卫星定位系统使用的坐标系。  
GCJ02：又称火星坐标系，是由中国国家测绘局制定的地理坐标系统，是由WGS84加密后得到的坐标系。  
BD09：为百度坐标系，在GCJ02坐标系基础上再次加密。其中bd09ll表示百度经纬度坐标，bd09mc表示百度墨卡托米制坐标。  

本项目还包括一个GPX文件操作模块。

编译：

    mkdir build
    cd build
    cmake ..
    cmake --build . 

参见：  
http://lbsyun.baidu.com/index.php?title=TransformCoordinate  
http://www.360doc.com/content/16/0721/16/9200790_577327509.shtml
