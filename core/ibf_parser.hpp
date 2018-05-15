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

#ifndef IBF_PARSER_H
#define IBF_PARSER_H

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#include <iostream>
#include <map>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>


#include <iterator>
#include <array>

#include "config.hpp"
#include "core/utils.hpp"
#include "core/ibf_node.hpp"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

/**
 * @brief The ibf_parserer class
 *
 * This class will load, parse and store ibf data dump.
 */
class ibf_parserer
{
public:
    /**
     * @brief ibf_parserer
     * Constructor
     */
    ibf_parserer(const std::string& name);

    /**
     * @brief ibf_parserer
     * Destructor :)
     */
    ~ibf_parserer();

    /**
     * @brief test dump file load
     * @return faisle if any issues
     */
    bool load();

    /**
     * @brief parse brief IBF data dump
     * @param action requested action on the processed data
     * @param param action argument or parameter
     * @return
     */
    bool parse(int action, std::string param = "");

private:
    /**
     * @brief data_file_
     * Data file name to operate on.
     */
    std::string data_file_;

    /**
     * @brief configs_
     * Array for storing nodes id's.
     */
    std::array<std::string, 0> nodes_;

};

#endif // IBF_PARSER_H
