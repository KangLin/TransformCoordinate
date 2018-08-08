/****************************************************************************
 *   Copyright (c) 2014 - 2016 Frederic Bourgeois <bourgeoislab@gmail.com>  *
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
 
#ifndef _UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Changes an environment variable
 * @param name Variable
 * @param value Value
 */
void UTILS_setenv(const char *name, const char *value);

/**
 * @brief Seletes the variable name from the environment
 * @param name Variable
 */
void UTILS_unsetenv(const char *name);

/**
 * @brief Converts a string to a double
 * @param str String
 * @return Double value
 */
double UTILS_atof(const char *str);

#ifdef __cplusplus
}
#endif

#endif // _UTILS_H
