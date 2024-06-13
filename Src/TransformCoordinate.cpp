// 作者：康林<kl222@126.com>

#include <math.h>
#include <string.h>

#include "TransformCoordinate.h"

///////////////////////////////////////////////////////////////////
/**
 * @class dpoint_t
 * @brief double point type.
 */
struct dpoint_t {
    double x;
    double y;
    dpoint_t() :
        x(0), y(0) {
    }
    dpoint_t(double xx, double yy) :
        x(xx), y(yy) {
    }
    dpoint_t(const dpoint_t& rh) :
        x(rh.x), y(rh.y) {
    }
};

dpoint_t _conv_(const dpoint_t& fromPoint, double factor[]) ;

dpoint_t ll2mc(const dpoint_t& point) ;

int ll2mc(const dpoint_t& pt, dpoint_t* res) ;

class GcjEncryptor {
private:
    double casm_rr;
    unsigned int casm_t1;
    unsigned int casm_t2;
    double casm_x1;
    double casm_y1;
    double casm_x2;
    double casm_y2;
    double casm_f;
    
    double yj_sin2(double x);
    double Transform_yj5(double x, double y) ;
    double Transform_yjy5(double x, double y);
    double Transform_jy5(double x, double xx) ;
    double Transform_jyj5(double x, double yy) ;
    double r_yj();
    double random_yj() ;
    void IniCasm(unsigned int w_time, unsigned int w_lng, unsigned int w_lat);
    unsigned int wgtochina_lb(int wg_flag, unsigned int wg_lng,
                              unsigned int wg_lat, int wg_heit, int wg_week, unsigned int wg_time,
                              unsigned int *china_lng, unsigned int *china_lat);
    
private:
    unsigned int _iix_, _iiy_;
    unsigned int _iox_, _ioy_;
    static const int _COFF_ = 3686400;
    
public:
    GcjEncryptor() {
        casm_rr = 0;
        casm_t1 = 0;
        casm_t2 = 0;
        casm_x1 = 0;
        casm_y1 = 0;
        casm_x2 = 0;
        casm_y2 = 0;
        casm_f = 0;
    }
    ~GcjEncryptor() {}
    
    int encrypt(const dpoint_t& pt, dpoint_t* res) {
        if( !res ) {
            return -1;
        }
        _iix_ = static_cast<unsigned int>(pt.x * _COFF_);
        _iiy_ = static_cast<unsigned int>(pt.y * _COFF_);
        if(this->wgtochina_lb(1, _iix_, _iiy_, 1,  0, 0, &_iox_, &_ioy_)) {
            return -2;
        }
        res->x = static_cast<double>(_iox_) / static_cast<double>(_COFF_);
        res->y = static_cast<double>(_ioy_) / static_cast<double>(_COFF_);
        return 0;
    }
    
    int encrypt(double oldx, double oldy, double &newx, double &newy) {
        _iix_ = static_cast<unsigned int>(oldx * _COFF_);
        _iiy_ = static_cast<unsigned int>(oldy * _COFF_);
        if(this->wgtochina_lb(1, _iix_, _iiy_, 1,  0, 0, &_iox_, &_ioy_)) {
            return -2;
        }
        newx = static_cast<double>(_iox_) / static_cast<double>(_COFF_);
        newy = static_cast<double>(_ioy_) / static_cast<double>(_COFF_);
        return 0;
    }
};

//geometric 
const long double _GRID_RADIX_ = 3E3;
const long double _MAX_dR_ = 2E-5;
const long double _MAX_dT_ = 3E-6;
const double _LL2RAD_ = 0.0174532925194;
const long double _OFFSET_X_ = 0.0065;
const long double _OFFSET_Y_ = 0.0060;

const double EARTHRADIUS = 6370996.81;
const double MCBAND[6] = { 12890594.86, 8362377.87, 5591021, 3481989.83,
                           1678043.12, 0 };
const double LLBAND[6] = { 75, 60, 45, 30, 15, 0 };

const double MC2LL[][10] = { { 1.410526172116255e-008, 8.983055096488720e-006,
                               -1.99398338163310, 2.009824383106796e+002, -1.872403703815547e+002,
                               91.60875166698430, -23.38765649603339, 2.57121317296198,
                               -0.03801003308653, 1.733798120000000e+007 },
                             { -7.435856389565537e-009,
                               8.983055097726239e-006, -0.78625201886289, 96.32687599759846,
                               -1.85204757529826, -59.36935905485877, 47.40033549296737,
                               -16.50741931063887, 2.28786674699375, 1.026014486000000e+007 }, 
                             {
                                 -3.030883460898826e-008, 8.983055099835780e-006, 0.30071316287616,
                                 59.74293618442277, 7.35798407487100, -25.38371002664745,
                                 13.45380521110908, -3.29883767235584, 0.32710905363475,
                                 6.856817370000000e+006 }, 
                             { -1.981981304930552e-008,
                               8.983055099779535e-006, 0.03278182852591, 40.31678527705744,
                               0.65659298677277, -4.44255534477492, 0.85341911805263,
                               0.12923347998204, -0.04625736007561, 4.482777060000000e+006 }, 
                             {
                                 3.091913710684370e-009, 8.983055096812155e-006, 0.00006995724062,
                                 23.10934304144901, -0.00023663490511, -0.63218178102420,
                                 -0.00663494467273, 0.03430082397953, -0.00466043876332,
                                 2.555164400000000e+006 },
                             { 2.890871144776878e-009,
                               8.983055095805407e-006, -0.00000003068298, 7.47137025468032,
                               -0.00000353937994, -0.02145144861037, -0.00001234426596,
                               0.00010322952773, -0.00000323890364, 8.260885000000000e+005 } };

const double LL2MC[][10] = { { -0.00157021024440, 1.113207020616939e+005,
                               1.704480524535203e+015, -1.033898737604234e+016,
                               2.611266785660388e+016, -3.514966917665370e+016,
                               2.659570071840392e+016, -1.072501245418824e+016,
                               1.800819912950474e+015, 82.50000000000000 }, 
                             { 8.277824516172526e-004,
                               1.113207020463578e+005, 6.477955746671608e+008,
                               -4.082003173641316e+009, 1.077490566351142e+010,
                               -1.517187553151559e+010, 1.205306533862167e+010,
                               -5.124939663577472e+009, 9.133119359512032e+008, 67.50000000000000 }, 
                             {
                                 0.00337398766765, 1.113207020202162e+005, 4.481351045890365e+006,
                                 -2.339375119931662e+007, 7.968221547186455e+007,
                                 -1.159649932797253e+008, 9.723671115602145e+007,
                                 -4.366194633752821e+007, 8.477230501135234e+006, 52.50000000000000 },
                             {
                                 0.00220636496208, 1.113207020209128e+005, 5.175186112841131e+004,
                                 3.796837749470245e+006, 9.920137397791013e+005,
                                 -1.221952217112870e+006, 1.340652697009075e+006,
                                 -6.209436990984312e+005, 1.444169293806241e+005, 37.50000000000000 }, 
                             {
                                 -3.441963504368392e-004, 1.113207020576856e+005,
                                 2.782353980772752e+002, 2.485758690035394e+006, 6.070750963243378e+003,
                                 5.482118345352118e+004, 9.540606633304236e+003,
                                 -2.710553267466450e+003, 1.405483844121726e+003, 22.50000000000000 },
                             {-3.218135878613132e-004, 1.113207020701615e+005, 0.00369383431289,
                              8.237256402795718e+005, 0.46104986909093,
                              2.351343141331292e+003, 1.58060784298199, 8.77738589078284,
                              0.37238884252424, 7.45000000000000 } };


/**
 * @brief
 */
dpoint_t _conv_(const dpoint_t& fromPoint, double factor[]) {
    dpoint_t toPoint;
    toPoint.x = factor[0] + factor[1] * fabs(fromPoint.x);
    double temp = fabs(fromPoint.y) / factor[9];
    toPoint.y = factor[2] + factor[3] * temp + factor[4] * temp * temp
            + factor[5] * temp * temp * temp + factor[6] * temp * temp * temp
            * temp + factor[7] * temp * temp * temp * temp * temp + factor[8]
            * temp * temp * temp * temp * temp * temp;
    toPoint.x *= (fromPoint.x < 0 ? -1 : 1);
    toPoint.y *= (fromPoint.y < 0 ? -1 : 1);
    return toPoint;
}

/**
 * @brief
 */
inline long double _get_delta_r_(long double y0) {
    return sin(y0 * _GRID_RADIX_ * _LL2RAD_) * _MAX_dR_;
}
/**
 * @brief
 */
inline long double _get_delta_t_(long double x0) {
    return cos(x0 * _GRID_RADIX_ * _LL2RAD_) * _MAX_dT_;
}

int bd_encrypt(const dpoint_t& pt, dpoint_t* res) {
    if (!res) {
        return -1;
    }
    
    long double x0 = static_cast<long double> (pt.x);
    long double y0 = static_cast<long double> (pt.y);
    long double r0 = sqrt(x0 * x0 + y0 * y0);
    long double theta0 = atan2(y0, x0);
    long double r1 = r0 + _get_delta_r_(y0);
    long double theta1 = theta0 + _get_delta_t_(x0);
    long double x1(r1 * cos(theta1)), y1(r1 * sin(theta1));
    
    res->x = static_cast<double> (x1 + _OFFSET_X_);
    res->y = static_cast<double> (y1 + _OFFSET_Y_);
    
    return 0;
}

/**
 * @brief transform from lat-lon to mercator.
 * @param pt point.
 * @return result point.
 */
dpoint_t ll2mc(const dpoint_t& point) {
    dpoint_t temp;
    temp.x = point.x;
    if(temp.x > 180.0) {
        temp.x = 180.0;
    } else if(temp.x < -180.0) {
        temp.x  = -180.0;
    }
    
    temp.y = point.y;
    if (temp.y < 1E-7 && temp.y >= 0.0) {
        temp.y = 1E-7;
    } else if(temp.y < 0 && temp.y > -1.0E-7) {
        temp.y = -1E-7;
    } else if(temp.y > 74) {
        temp.y = 74;
    } else if(temp.y < -74) {
        temp.y = -74;
    }
    
    double factor[10] = { 0 };
    unsigned int i = 0;
    for (i = 0; i < sizeof(LLBAND) / sizeof(double); i++) {
        if (fabs(temp.y) > LLBAND[i]) {
            memcpy(factor, LL2MC[i], sizeof(factor));
            break;
        }
    }
    return _conv_(temp, factor);
}

/**
 * @brief transform from lat-lon to mercator.
 * @param pt point.
 * @param res return point ptr.
 * @return
 *      - 0     -   ok.
 *      - <0    -   failed.
 */
int ll2mc(const dpoint_t& pt, dpoint_t* res) {
    if (!res) {
        return -1;
    }
    (*res) = ll2mc(pt);
    return 0;
}

/**
 * @brief transform from mercator to lat-lon.
 * @param pt point.
 * @return result point.
 */
dpoint_t mc2ll(const dpoint_t& point) {
    dpoint_t temp;
    temp.x = point.x;
    if(temp.x > 20037508.342) {
        temp.x = 20037508.342;
    } else if(temp.x < -20037508.342) {
        temp.x = -20037508.342;
    }
    temp.y = point.y;
    if (temp.y < 1E-6 && temp.y >= 0) {
        temp.y = 1E-6;
    } else if(temp.y < 0 && temp.y > -1.0E-6) {
        temp.y = -1E-6;
    } else if(temp.y > 20037508.342) {
        temp.y = 20037508.342;
    } else if(temp.y < -20037508.342) {
        temp.y = -20037508.342;
    }
    
    double factor[10] = { 0 };
    unsigned int i = 0;
    for (i = 0; i < sizeof(MCBAND) / sizeof(double); i++) {
        if (fabs(temp.y) > MCBAND[i]) {
            memcpy(factor, MC2LL[i], sizeof(factor));
            break;
        }
    }
    return _conv_(temp, factor);
}

/**
 * @brief transform from mercator to lat-lon.
 * @param pt point.
 * @param res return point ptr.
 * @return
 *      - 0     -   ok.
 *      - <0    -   failed.
 */
int mc2ll(const dpoint_t& pt, dpoint_t* res) {
    if (!res) {
        return -1;
    }
    (*res) = mc2ll(pt);
    return 0;
}

/**
 * @brief baidu decrypt.
 * @param pt point.
 * @param res return point ptr.
 * @return
 *      - 0     -   ok.
 *      - <0    -   failed.
 */
int bd_decrypt(const dpoint_t& pt, dpoint_t* res) {
    if (!res) {
        return -1;
    }
    
    long double x0 = static_cast<long double> (pt.x) - _OFFSET_X_;
    long double y0 = static_cast<long double> (pt.y) - _OFFSET_Y_;
    long double r0 = sqrt(x0 * x0 + y0 * y0);
    long double theta0 = atan2(y0, x0);
    long double r1 = r0 - _get_delta_r_(y0);
    long double theta1 = theta0 - _get_delta_t_(x0);
    long double x1(r1 * cos(theta1)), y1(r1 * sin(theta1));
    
    res->x = static_cast<double> (x1);
    res->y = static_cast<double> (y1);
    
    return 0;
}

/**
 * @brief transform coordinates from baidu09 system to wgs84 system;
 * @param pt point.
 * @param res return point ptr.
 * @return
 *      - 0     -   ok.
 *      - <0    -   failed.
 */
int bd09_to_wgs84(const dpoint_t& pt, dpoint_t* res)
{
    dpoint_t bd_pt;
    if(mc2ll(pt, &bd_pt) < 0) {
        return -1;
    }
    if(bd_decrypt(bd_pt, res) < 0) {
        return -2;
    }
    
    return 0;
}

double GcjEncryptor::yj_sin2(double x) {
    double tt;
    double ss;
    int ff;
    double s2;
    int cc;
    ff = 0;
    if (x < 0) {
        x = -x;
        ff = 1;
    }
    cc = static_cast<int>(x / 6.28318530717959);
    tt = x - cc * 6.28318530717959;
    if (tt > 3.1415926535897932) {
        tt = tt - 3.1415926535897932;
        if (ff == 1)
            ff = 0;
        else if (ff == 0)
            ff = 1;
    }
    x = tt;
    ss = x;
    s2 = x;
    tt = tt * tt;
    s2 = s2 * tt;
    ss = ss - s2 * 0.166666666666667;
    s2 = s2 * tt;
    ss = ss + s2 * 8.33333333333333E-03;
    s2 = s2 * tt;
    ss = ss - s2 * 1.98412698412698E-04;
    s2 = s2 * tt;
    ss = ss + s2 * 2.75573192239859E-06;
    s2 = s2 * tt;
    ss = ss - s2 * 2.50521083854417E-08;
    if (ff == 1)
        ss = -ss;
    return ss;
}
double GcjEncryptor::Transform_yj5(double x, double y) {
    double tt;
    tt = 300 + 1 * x + 2 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(sqrt(x
                                                                           * x));
    tt = tt + (20 * yj_sin2(18.849555921538764 * x) + 20 * yj_sin2(
                   6.283185307179588 * x)) * 0.6667;
    tt = tt + (20 * yj_sin2(3.141592653589794 * x) + 40 * yj_sin2(
                   1.047197551196598 * x)) * 0.6667;
    tt = tt + (150 * yj_sin2(0.2617993877991495 * x) + 300 * yj_sin2(
                   0.1047197551196598 * x)) * 0.6667;
    return tt;
}
double GcjEncryptor::Transform_yjy5(double x, double y) {
    double tt;
    tt = -100 + 2 * x + 3 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(sqrt(x
                                                                            * x));
    tt = tt + (20 * yj_sin2(18.849555921538764 * x) + 20 * yj_sin2(
                   6.283185307179588 * x)) * 0.6667;
    tt = tt + (20 * yj_sin2(3.141592653589794 * y) + 40 * yj_sin2(
                   1.047197551196598 * y)) * 0.6667;
    tt = tt + (160 * yj_sin2(0.2617993877991495 * y) + 320 * yj_sin2(
                   0.1047197551196598 * y)) * 0.6667;
    return tt;
}
double GcjEncryptor::Transform_jy5(double x, double xx) {
    double n;
    double a;
    double e;
    a = 6378245;
    e = 0.00669342;
    n = sqrt(1 - e * yj_sin2(x * 0.0174532925199433) * yj_sin2(x
                                                               * 0.0174532925199433));
    n = (xx * 180) / (a / n * cos(x * 0.0174532925199433) * 3.1415926);
    return n;
}
double GcjEncryptor::Transform_jyj5(double x, double yy) {
    double m;
    double a;
    double e;
    double mm;
    a = 6378245;
    e = 0.00669342;
    mm = 1 - e * yj_sin2(x * 0.0174532925199433) * yj_sin2(x
                                                           * 0.0174532925199433);
    m = (a * (1 - e)) / (mm * sqrt(mm));
    return (yy * 180) / (m * 3.1415926);
}
double GcjEncryptor::r_yj() {
    int casm_a;
    int casm_c;
    casm_a = 314159269;
    casm_c = 453806245;
    return 0;
}
double GcjEncryptor::random_yj() {
    int t;
    int casm_a;
    int casm_c;
    casm_a = 314159269;
    casm_c = 453806245;
    casm_rr = casm_a * casm_rr + casm_c;
    t = static_cast<int>(casm_rr / 2);
    casm_rr = casm_rr - t * 2;
    casm_rr = casm_rr / 2;
    return (casm_rr);
}
void GcjEncryptor::IniCasm(unsigned int w_time, unsigned int w_lng, unsigned int w_lat) {
    int tt;
    casm_t1 = w_time;
    casm_t2 = w_time;
    tt = static_cast<int>(w_time / 0.357);
    casm_rr = w_time - tt * 0.357;
    if (w_time == 0)
        casm_rr = 0.3;
    casm_x1 = w_lng;
    casm_y1 = w_lat;
    casm_x2 = w_lng;
    casm_y2 = w_lat;
    casm_f = 3;
}
unsigned int GcjEncryptor::wgtochina_lb(int wg_flag, unsigned int wg_lng,
                                        unsigned int wg_lat, int wg_heit, int wg_week, unsigned int wg_time,
                                        unsigned int *china_lng, unsigned int *china_lat) {
    double x_add;
    double y_add;
    double h_add;
    double x_l;
    double y_l;
    double casm_v;
    double t1_t2;
    double x1_x2;
    double y1_y2;
    
    if (wg_heit > 5000) {
        *china_lng = 0;
        *china_lat = 0;
        return 0xFFFF95FF;
    }
    x_l = wg_lng;
    x_l = x_l / 3686400.0;
    y_l = wg_lat;
    y_l = y_l / 3686400.0;
    if (x_l < 72.004) {
        *china_lng = 0;
        *china_lat = 0;
        return 0xFFFF95FF;
    }
    if (x_l > 137.8347) {
        *china_lng = 0;
        *china_lat = 0;
        return 0xFFFF95FF;
    }
    if (y_l < 0.8293) {
        *china_lng = 0;
        *china_lat = 0;
        return 0xFFFF95FF;
    }
    if (y_l > 55.8271) {
        *china_lng = 0;
        *china_lat = 0;
        return 0xFFFF95FF;
    }
    //if (wg_flag == 0) 
    IniCasm(wg_time, wg_lng, wg_lat);
    if(0)
    {
        IniCasm(wg_time, wg_lng, wg_lat);
        *china_lng = wg_lng;
        *china_lat = wg_lat;
        return 0x00000000;
    }
    
    casm_t2 = wg_time;
    t1_t2 = (double) (casm_t2 - casm_t1) / 1000.0;
    if (t1_t2 <= 0) {
        casm_t1 = casm_t2;
        casm_f = casm_f + 1;
        casm_x1 = casm_x2;
        casm_f = casm_f + 1;
        casm_y1 = casm_y2;
        casm_f = casm_f + 1;
    } else {
        if (t1_t2 > 120) {
            if (casm_f == 3) {
                casm_f = 0;
                casm_x2 = wg_lng;
                casm_y2 = wg_lat;
                x1_x2 = casm_x2 - casm_x1;
                y1_y2 = casm_y2 - casm_y1;
                casm_v = sqrt(x1_x2 * x1_x2 + y1_y2 * y1_y2) / t1_t2;
                if (casm_v > 3185) {
                    *china_lng = 0;
                    *china_lat = 0;
                    return (0xFFFF95FF);
                }
                
            }
            casm_t1 = casm_t2;
            casm_f = casm_f + 1;
            casm_x1 = casm_x2;
            casm_f = casm_f + 1;
            casm_y1 = casm_y2;
            casm_f = casm_f + 1;
        }
    }
    x_add = Transform_yj5(x_l - 105, y_l - 35);
    y_add = Transform_yjy5(x_l - 105, y_l - 35);
    h_add = wg_heit;
    
    x_add = x_add + h_add * 0.001 + yj_sin2(wg_time * 0.0174532925199433)
            + random_yj();
    y_add = y_add + h_add * 0.001 + yj_sin2(wg_time * 0.0174532925199433)
            + random_yj();
    *china_lng = static_cast<int>((x_l + Transform_jy5(y_l, x_add)) * 3686400);
    *china_lat = static_cast<int>((y_l + Transform_jyj5(y_l, y_add)) * 3686400);
    return (0x00000000);
}

double dis(double x1,double y1,double x2,double y2)
{
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}
GcjEncryptor g_gcj_encryptor;

int gcj_decrypt(dpoint_t oldxy, dpoint_t *newxy) {
    if (newxy == NULL) {
        return -1;
    }
    double xmid = oldxy.x, ymid = oldxy.y;
    double cellsize = 1;
    double realgcjx = oldxy.x, realgcjy = oldxy.y;
    double x1 = xmid - cellsize,x1new = 0;
    double y1 = ymid + cellsize,y1new = 0;
    double x2 = xmid - cellsize,x2new = 0;
    double y2 = ymid - cellsize,y2new = 0;
    double x3 = xmid + cellsize,x3new = 0;
    double y3 = ymid - cellsize,y3new = 0;
    double x4 = xmid + cellsize,x4new = 0;
    double y4 = ymid + cellsize,y4new = 0;
    double xmidnew,ymidnew;
    double gpsx,gpsy;
    int res = -1;
    g_gcj_encryptor.encrypt(xmid,ymid,xmidnew,ymidnew);
    if (dis(xmidnew,ymidnew,realgcjx,realgcjy) <= 1e-6)
    {
        gpsx = xmid;
        gpsy = ymid;
        res = 0;
    }
    else
    {
        while(1)
        {//ÓÐµü´úµÄ±ØÒª
            double x1 = xmid - cellsize,x1new = 0;
            double y1 = ymid + cellsize,y1new = 0;
            double x2 = xmid - cellsize,x2new = 0;
            double y2 = ymid - cellsize,y2new = 0;
            double x3 = xmid + cellsize,x3new = 0;
            double y3 = ymid - cellsize,y3new = 0;
            double x4 = xmid + cellsize,x4new = 0;
            double y4 = ymid + cellsize,y4new = 0;
            g_gcj_encryptor.encrypt(x1,y1,x1new,y1new);
            g_gcj_encryptor.encrypt(x2,y2,x2new,y2new);
            g_gcj_encryptor.encrypt(x3,y3,x3new,y3new);
            g_gcj_encryptor.encrypt(x4,y4,x4new,y4new);
            double dis1 = dis(x1new,y1new,realgcjx,realgcjy);
            double dis2 = dis(x2new,y2new,realgcjx,realgcjy);
            double dis3 = dis(x3new,y3new,realgcjx,realgcjy);
            double dis4 = dis(x4new,y4new,realgcjx,realgcjy);
            if (dis1 < 1e-6)
            {
                gpsx = x1;
                gpsy = y1;
                res = 0;
                break;
            }
            if (dis2 < 1e-6)
            {
                gpsx = x2;
                gpsy = y2;
                res = 0;
                break;
            }
            if (dis3 < 1e-6)
            {
                gpsx = x3;
                gpsy = y3;
                res = 0;
                break;
            }
            if (dis4 < 1e-6)
            {
                gpsx = x4;
                gpsy = y4;
                res = 0;
                break;
            }
            //ËÄ¸ö¶¼²»Îª0
            double w1 = 1/dis1, w2 = 1/dis2, w3 = 1/dis3, w4 = 1/dis4;
            xmid = ( x1 * w1 + x2 * w2 + x3 * w3 + x4 * w4 )/(w1 + w2 + w3 + w4);
            ymid = ( y1 * w1 + y2 * w2 + y3 * w3 + y4 * w4 )/(w1 + w2 + w3 + w4);
            g_gcj_encryptor.encrypt(xmid,ymid,xmidnew,ymidnew);
            double dismid = dis(xmidnew,ymidnew,realgcjx,realgcjy);
            if ( dismid <= 1e-6)
            {
                gpsx = xmid;
                gpsy = ymid;
                res = 0;
                break;
            }
            cellsize *= 0.6;
            if (cellsize < 1e-6)
            {
                gpsx = gpsy = -1;
                res = -1;
                break;
            }
        }
    }
    
    if (res == 0)
    {
        newxy->x = gpsx;
        newxy->y = gpsy;
    }
    return res;
}

int TransformCoordinate(double oldx, double oldy,
                        double &newx, double &newy,
                        _COORDINATE from, _COORDINATE to)
{    
    if (from == to) {
        newx = oldx;
        newy = oldy;
        return 0;
    }
    
    dpoint_t oldxy(oldx, oldy), newxy;
    switch(from)
    {
    case WGS84:
        /* WGS84 to gcj02 */
        if (g_gcj_encryptor.encrypt(oldxy, &oldxy) != 0) {
            return -1;
        }
        break;
    case BD09MC:
        oldxy = mc2ll(oldxy);
    case BD09LL:
        /* BD09LL to gcj02 */
        if (bd_decrypt(oldxy, &oldxy) != 0) {
            return -1;
        }
        break;
    default:
        break;
    }
    
    switch (to) {
    case WGS84:
        /* gcj02 to wgs84 */
        if (gcj_decrypt(oldxy, &oldxy) != 0) {
            return -1;
        }
        break;
    case BD09LL:
    case BD09MC:
        /* gcj02 to BD09LL */
        if (bd_encrypt(oldxy, &oldxy) != 0) {
            return -1;
        }
    default:
        break;
    }
        
    if (BD09MC == to) {
        newxy = ll2mc(oldxy);
    } else {
        newxy = oldxy;
    }
    
    newx = newxy.x;
    newy = newxy.y;
    return 0;
}

#ifdef WITH_GPXMODEL
#include "GpxModel/gpx_model.h"
#include <iostream>
#include <string>

int TransformCoordinateFiles(const char* szSrc, const char* szDst,
                             _COORDINATE from, _COORDINATE to, bool bIgnoreError)
{
    if(NULL == szSrc || NULL == szDst)
        return -1;
    
    GPX_model gpx("TransformCoordinate");
    if(GPX_model::GPXM_OK != gpx.load(szSrc, GPX_model::GPXM_FILE_AUTOMATIC, true))
    {
        std::cout << "Open file " << szSrc << " fail." << std::endl;
        return -2;
    }
     
    gpx.metadata.desc = "Transform coordinate from " + gCoordinateDescription[from] + " to " + gCoordinateDescription[to];
    GPX_linkType linkType;
    linkType.href = "https://github.com/KangLin/coordtrans";
    linkType.text = "Transform coordinate project. Author: Kang Lin <kl222@126.com>";
    linkType.type = "text/html";
    //*
    gpx.metadata.author.link = linkType;
    gpx.metadata.author.name = "Kang Lin";
    gpx.metadata.author.email.id = "kl222";
    gpx.metadata.author.email.domain = "126.com";
    //*/
    gpx.metadata.links.push_back(linkType);
    
    bool bSave = false;
    std::vector<GPX_trkType>::iterator it;
    for(it = gpx.trk.begin(); it != gpx.trk.end(); it++)
    {
        std::vector<GPX_trksegType>::iterator itSeg;
        for(itSeg = it->trkseg.begin(); itSeg != it->trkseg.end(); itSeg++)
        {
            std::vector<GPX_wptType>::iterator itWpt;
            for(itWpt = itSeg->trkpt.begin(); itWpt != itSeg->trkpt.end();
                itWpt++)
            {
                double x, y;
                int nRet = TransformCoordinate(itWpt->longitude,
                                               itWpt->latitude,
                                               x, y, from, to);
                if(nRet) {
                    std::cout << "Transform coordinat fail: longitude:"
                              << itWpt->longitude
                              << "latitude:" << itWpt->latitude;
                    if(bIgnoreError)
                        continue;
                    else
                        return nRet;
                } else {
                    itWpt->longitude = x;
                    itWpt->latitude = y;
                    bSave = true;
                }
            }
        }
    }
    
    if(bSave)
        gpx.save(szDst);
    
    return 0;
}
#endif
