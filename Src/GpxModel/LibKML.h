#ifndef LIBKML_H
#define LIBKML_H

#include "gpx_model.h"

class CLibKML
{
public:
    CLibKML();
    
     /**
     * @brief Parses a GPX file and fills the GPX model
     * @param fp File handler to the opened GPX file
     * @param gpxm GPX_model
     * @param overwriteMetadata If true the metadata of GPX_model is overwritten
     * @return Return code, GPXM_OK on success
     */
    static GPX_model::retCode_e load(ifstream* fp, GPX_model* gpxm, bool overwriteMetadata = false);

    /**
     * @brief Saves a GPX file representing the GPX model
     * @param fp File handler to the opened GPX file
     * @param gpxm GPX model to write
     * @return Return code, GPXM_OK on success
     */
    static GPX_model::retCode_e save(ofstream* fp, const GPX_model* gpxm);
};

#endif // LIBKML_H
