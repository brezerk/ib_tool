/***************************************************************************
 *   Copyright (C) 2018 by Oleksii S. Malakhov <brezerk@gmail.com>         *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#ifndef UTILS_HPP
#define UTILS_HPP

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#include <iostream>
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "config.hpp"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#ifdef _OS_LINUX_
const boost::filesystem::path pwd = boost::filesystem::current_path();
//const boost::filesystem3::path pwd = boost::filesystem3::path(APP_PREF);
#else
const boost::filesystem::path pwd = boost::filesystem::path(APP_PREF);
//const boost::filesystem3::path pwd = boost::filesystem3::current_path();
#endif

/**
 * @brief getPathTo
 * Concat current working directory with requested path
 * @param path_to an a requested resource path
 * @return full path to a requested resource
 */
const std::string getPathTo(const std::string & path_to);

/**
 * @brief to_array
 * This is used to convert string separated by "," to
 * a vector of strings.
 *
 * @param s and a string string :)
 */
template<typename T> std::vector<T> to_array(const std::string& s) {
    std::vector<T> result;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, ',')){
        boost::trim(item);
        result.push_back(boost::lexical_cast<T>(item));
    }
    return result;
}

#endif // UTILS_HPP
