/****************************************************************************
 *   Copyright (c) 2014 - 2015 Frederic Bourgeois <bourgeoislab@gmail.com>  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with This program. If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

#ifndef _NMEAFILE_H_
#define _NMEAFILE_H_

#include <string>
#include <fstream>
#include "gpx_model.h"

using namespace std;

/**
 * @ingroup GPX_model
 * @{
 */

/**
 * @namespace NMEAFile
 *
 * @brief Functions to load NMEA files.
 *
 * This file provides a function to read a NMEA file.
 * While reading the NMEA file a GPX_model structure is filled.
 *
 * @see http://www.gpsinformation.org/dale/nmea.htm
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.3
 * @date 30 Jul 2016
 */
namespace NMEAFile
{
    /**
     * @brief Parses a NMEA file and fills the GPX model structure
     * @param fp File handler to the opened ACT file
     * @param gpxm GPX_model
     * @param name Name of the NMEA file without extension
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e load(ifstream* fp, GPX_model* gpxm, const string& name = "");
}

/** @} GPX_model */

#endif // _NMEAFILE_H_
