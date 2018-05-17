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

#ifndef IBF_NODE_H
#define IBF_NODE_H

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#include <iostream>
#include <map>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include <iterator>
#include <array>

#include "config.hpp"
#include "core/utils.hpp"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

/**
 * @brief The ibf_node class.
 *
 * Load and parse node dump info
 */
class ibf_node
{
public:
    /**
     * @brief ibf_node
     * Constructor
     * @param dump data
     */
    ibf_node(const std::string& dump);

    /**
     * @brief ibf_node
     * Destructor :)
     */
    ~ibf_node();

    /**
     * @brief load brief IBF data dump
     * @return true if ok
     */
    bool load();

    /**
     * @brief print node info (Node type, Number of ports, Node GUID, Port GUID, Node description, LID)
     */
    void print();

    /**
     * @brief get nodeGUID
     * @return nodeGUID
     */
    std::string getNodeGUID() {
        return this->node_guid_;
    }

    /**
     * @brief get local port
     * @return local port
     */
    uint32_t getLocalPort() {
        return this->pn_;
    }

    /**
     * @brief get local port GUID
     * @return local port GUID
     */
    std::string getLocalPortGUID() {
        return this->port_guid_;
    }

    /**
     * @brief get node ports count
     * @return node ports count
     */
    uint32_t getPorts() {
        return this->ports_;
    }

private:
    /**
     * @brief data_file_
     * Data file name to operate on.
     */
    std::string dump_;

    /**
     * @brief type_
     * Node type CA|DW|CA-CM
     */
    std::string type_;

    /**
     * @brief ports_
     * Node pots count
     */
    uint32_t ports_;

    /**
     * @brief system_guid_
     * System guid
     */
    std::string system_guid_;

    /**
     * @brief node_guid_
     * node guid
     */
    std::string node_guid_;

    /**
     * @brief port_guid_
     * port guid
     */
    std::string port_guid_;

    /**
     * @brief venid_
     * vendor id
     */
    std::string venid_;

    /**
     * @brief dev_id_
     * device id
     */
    std::string devid_;

    /**
     * @brief rev_
     * revision
     */
    std::string rev_;

    /**
     * @brief dev_
     * device
     */
    std::string dev_;

    /**
     * @brief name_
     * node name or description
     */
    std::string name_;

    /**
     * @brief lid_
     * lid
     */
    std::string lid_;

    /**
     * @brief pn_
     * pn
     */
    uint32_t pn_;
};

#endif // IBF_NODE_H
