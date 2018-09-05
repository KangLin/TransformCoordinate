#ifndef _COORDTRANS
#define _COORDTRANS

#include <math.h>
#include <string>

#ifdef WIN32
    #ifdef DLL_EXPORT
        #define DLL_API __declspec(dllexport)
    #else
        #define DLL_API __declspec(dllimport)
    #endif
#else
    #define DLL_API
#endif

/*
WGS84：为一种大地坐标系，也是目前广泛使用的GPS全球卫星定位系统使用的坐标系。  
GCJ02：又称火星坐标系，是由中国国家测绘局制定的地理坐标系统，是由WGS84加密后得到的坐标系。  
BD09：为百度坐标系，在GCJ02坐标系基础上再次加密。其中bd09ll表示百度经纬度坐标，bd09mc表示百度墨卡托米制坐标。  
参见：http://lbsyun.baidu.com/index.php?title=coordinate
http://www.360doc.com/content/16/0721/16/9200790_577327509.shtml
*/
enum _COORDINATE{
    WGS84,
    GCJ02,
    BD09LL, //百度经纬度坐标  
    BD09MC  //百度墨卡托米制坐标  
};

static std::string gCoordinateDescription[] = {"WGS84", "GCJ02", "BD09LL", "BD09MC"};   
DLL_API int TransformCoordinate(double oldx,
                                double oldy,
                                double &newx,
                                double &newy,
                                _COORDINATE from = WGS84,
                                _COORDINATE to = GCJ02);

#ifdef BUILD_GPXMODEL
DLL_API int TransformCoordinateFiles(const char *szSrc,
                                     const char *szDst,
                                     _COORDINATE from = WGS84,
                                     _COORDINATE to = GCJ02);
#endif //BUILD_GPXMODEL

#endif
