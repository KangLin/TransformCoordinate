#include "LibKML.h"

CLibKML::CLibKML()
{
}

GPX_model::retCode_e CLibKML::load(ifstream *fp, GPX_model *gpxm, bool overwriteMetadata)
{
    GPX_model::retCode_e ret = GPX_model::GPXM_OK;
    
    return ret;
}

GPX_model::retCode_e CLibKML::save(ofstream *fp, const GPX_model *gpxm)
{
    GPX_model::retCode_e ret = GPX_model::GPXM_OK;
    
    return ret;
}
