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

#include "ibf_node.hpp"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8


ibf_node::ibf_node(const std::string& dump) {
    this->dump_ = dump;
}

ibf_node::~ibf_node() {

}

bool ibf_node::load() {
    // CA Ports:01 SystemGUID:7cfe900300327bfe NodeGUID:7cfe900300327bff PortGUID:7cfe900300327bff VenID:0002C9 DevID:1013 Rev:00000000 {DataDirect HCA-6} LID:0013 PN:01
    // SW Ports:25 SystemGUID:7cfe900300992080 NodeGUID:7cfe900300f2da60 PortGUID:7cfe900300f2da60 VenID:000002C9 DevID:CF080000 Rev:000000A0 {MF0;cs7500:CS7500/L17/U2} LID:00F9 PN:07
    // CA Ports:01 SystemGUID:7cfe9003004b0700 NodeGUID:7cfe9003004b0700 PortGUID:7cfe9003004b0700 VenID:0002C9 DevID:1013 Rev:00000000 {ime5 HCA-1} LID:0032 PN:01
    boost::regex getNodesRegEx("^(CA|SW) Ports:(\\d+) SystemGUID:([[:alnum:]]+) NodeGUID:([[:alnum:]]+) PortGUID:([[:alnum:]]+) VenID:([[:alnum:]]+) DevID:([[:alnum:]]+) Rev:([[:alnum:]]+) {(.*)} LID:([[:alnum:]]+) PN:([[:alnum:]]+)");
    boost::smatch getNodesRegExResults;
    boost::regex_match(this->dump_, getNodesRegExResults, getNodesRegEx);

#ifdef DEBUG
    std::cout << this->dump_ << std::endl;
#endif

    this->type_ = getNodesRegExResults[1];
    try {
        this->ports_ = boost::lexical_cast<int>(getNodesRegExResults[2]);
    } catch( boost::bad_lexical_cast const& ) {
        std::cerr << "Error: invalid port number: " << getNodesRegExResults[2] << std::endl;
        return false;
    }
    this->system_guid_ = getNodesRegExResults[3];
    this->node_guid_ = getNodesRegExResults[4];
    this->port_guid_ = getNodesRegExResults[5];
    this->venid_ = getNodesRegExResults[6];
    this->devid_ = getNodesRegExResults[7];
    this->rev_ = getNodesRegExResults[8];
    this->name_ = getNodesRegExResults[9];
    this->lid_ = getNodesRegExResults[10];
    this->pn_ = getNodesRegExResults[11];


    if (this->type_ == ""){
        std::cerr << "Error: invalid node type: " << this->type_ << std::endl;
        return false;
    }

    if (this->system_guid_ == ""){
        std::cerr << "Error: invalid SystemGUID: " << this->system_guid_ << std::endl;
        return false;
    }

    if (this->node_guid_ == ""){
        std::cerr << "Error: invalid NodeGUID: " << this->node_guid_ << std::endl;
        return false;
    }

    if (this->port_guid_ == ""){
        std::cerr << "Error: invalid PortGUID: " << this->port_guid_ << std::endl;
        return false;
    }

    if (this->venid_ == ""){
        std::cerr << "Error: invalid VenID: " << this->port_guid_ << std::endl;
        return false;
    }

    if (this->devid_ == ""){
        std::cerr << "Error: invalid DevID: " << this->devid_ << std::endl;
        return false;
    }

    if (this->rev_ == ""){
        std::cerr << "Error: invalid Rev: " << this->rev_ << std::endl;
        return false;
    }

    if (this->name_ == ""){
        std::cerr << "Error: invalid Name: " << this->name_ << std::endl;
        return false;
    }

    if (this->lid_ == ""){
        std::cerr << "Error: invalid LID: " << this->lid_ << std::endl;
        return false;
    }

    if (this->pn_ == ""){
        std::cerr << "Error: invalid PN: " << this->pn_ << std::endl;
        return false;
    }

    return true;
}

void ibf_node::print(){
    //(Node type, Number of ports, Node GUID, Port GUID, Node description, LID)
    std::cout << this->type_ << " " << this->ports_ << " " << this->node_guid_ << " " << this->name_ << " " << this->lid_ << std::endl;
}
