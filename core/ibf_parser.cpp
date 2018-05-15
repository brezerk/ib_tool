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

#include "ibf_parser.hpp"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8


ibf_parserer::ibf_parserer(const std::string& name) {
    this->data_file_ = name;
}

ibf_parserer::~ibf_parserer() {

}

bool ibf_parserer::load() {
    if (!boost::filesystem::exists(this->data_file_))
    {
        std::cerr << "Can't find file by path: " << this->data_file_ << std::endl;
        return false;
    }
    return true;
}

bool ibf_parserer::parse(int action, std::string param) {

    this->load();

    std::ifstream file(this->data_file_);
    std::string str;

    boost::regex getNodesRegEx("^{(.*)} {(.*)} (.*)$");
    boost::smatch getNodesRegExResults;

    bool load_ok;

    while (std::getline(file, str))
    {
        //std::cout << str << std::endl;
        boost::regex_match(str, getNodesRegExResults, getNodesRegEx);

#ifdef DEBUG
        std::cout << "Node 0: " << getNodesRegExResults[1] << std::endl;
        std::cout << "Node 1: " << getNodesRegExResults[2] << std::endl;
        std::cout << "Node Link: " << getNodesRegExResults[3] << std::endl;
#endif

        std::string ca_dump = getNodesRegExResults[1];
        boost::trim(ca_dump);

        ibf_node* ca_node = new ibf_node(ca_dump);
        load_ok = ca_node->load();


        if (!load_ok) {
            std::cerr << "Got error on loading dump string:" << std::endl << ca_dump << std::endl;
            return false;
        }

        switch (action) {
            case D_ACTION_NODES_ALL:
                ca_node->print();
            break;
            case D_ACTION_NODE:
                if (ca_node->getNodeGUID() == param) {
                    ca_node->print();
                }
            case D_ACTION_NODES_REG:
                boost::regex searchString(param);
                boost::smatch searchStringResults;
                if (boost::regex_match(ca_node->getNodeGUID(), searchStringResults, searchString)){
                    ca_node->print();
                }
            break;
        }

        delete(ca_node);

        std::string sw_dump = getNodesRegExResults[2];
        boost::trim(sw_dump);
        ibf_node* sw_node = new ibf_node(sw_dump);
        load_ok = sw_node->load();

        if (!load_ok) {
            std::cerr << "Got error on loading dump string:" << std::endl << sw_dump<< std::endl;
            return false;
        }

        switch (action) {
            case D_ACTION_NODES_ALL:
                sw_node->print();
            break;
            case D_ACTION_NODE:
                if (sw_node->getNodeGUID() == param) {
                    sw_node->print();
                }
            case D_ACTION_NODES_REG:
                boost::regex searchString(param);
                boost::smatch searchStringResults;
                if (boost::regex_match(sw_node->getNodeGUID(), searchStringResults, searchString)){
                    sw_node->print();
                }
            break;
        }
        delete(sw_node);
    }
    return true;
}
