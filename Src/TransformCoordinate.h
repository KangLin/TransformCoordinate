// 作者：康 林 <kl222@126.com>

#ifndef _COORDTRANS
#define _COORDTRANS

#include "transformcoordinate_export.h"
#include <string>

/*!
 * - WGS84：
 *   为一种大地坐标系，也是目前广泛使用的全球卫星定位系统（GPS）使用的坐标系。
 * - GCJ02：
 *   戏称火星坐标系，是由中国国家测绘局制定的地理坐标系统，是由WGS84加密后得到的坐标系。
 * - BD09：
 *   为百度坐标系，在GCJ02坐标系基础上再次加密。
 *   其中bd09ll表示百度经纬度坐标，bd09mc表示百度墨卡托米制坐标。
 */
enum _COORDINATE{
    WGS84,
    GCJ02,
    BD09LL, //百度经纬度坐标  
    BD09MC  //百度墨卡托米制坐标  
};

static std::string gCoordinateDescription[] = {"WGS84", "GCJ02", "BD09LL", "BD09MC"};   
TRANSFORMCOORDINATE_EXPORT int TransformCoordinate(
    double oldx,
    double oldy,
    double &newx,
    double &newy,
    _COORDINATE from = WGS84,
    _COORDINATE to = GCJ02);

#ifdef WITH_GPXMODEL
TRANSFORMCOORDINATE_EXPORT int TransformCoordinateFiles(
    const char *szSrc,
    const char *szDst,
    _COORDINATE from = WGS84,
    _COORDINATE to = GCJ02,
    bool bIgnoreError = false);
#endif //WITH_GPXMODEL

#endif
