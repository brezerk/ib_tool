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
    std::ifstream file;
    try {
        file.open(this->data_file_, std::ifstream::in);
    } catch (std::ifstream::failure e) {
        std::cerr << "Exception opening file: " << e.what() << std::endl;
        return false;
    }

    std::string str;

    //list of match guids
    std::set<std::string> node_guids;
    std::pair<std::set<std::string>::iterator,bool> node_guids_ret;

    boost::regex getNodesRegEx("^{(.*)} {(.*)} (.*)$");
    boost::smatch getNodesRegExResults;

    while (std::getline(file, str))
    {
        boost::regex_match(str, getNodesRegExResults, getNodesRegEx);
        for (int i=1; i<=3; i++){
        //int i=1;
#ifdef DEBUG
            std::cout << "Item " << i << " : " << getNodesRegExResults[i] << std::endl;
#endif
            if (i<3) {
                std::string dump = getNodesRegExResults[i];
                boost::trim(dump);

                std::unique_ptr<ibf_node> node(new ibf_node(dump));
                if (!node->load()) {
                    std::cerr << "Got error on loading dump string:" << std::endl << dump << std::endl;
                    file.close();
                    return false;
                }

                switch (action) {
                    case D_ACTION_NODES_ALL:
                        node_guids_ret = node_guids.insert(node->getNodeGUID());
                        if (node_guids_ret.second==true) node->print();
                    break;
                    case D_ACTION_NODE:
                        if (node->getNodeGUID() == param) {
                            node->print();
                            file.close();
                            return true;
                        }
                    break;
                    case D_ACTION_NODES_REG:
                        try {
                            boost::regex searchString(param);
                            boost::smatch searchStringResults;
                            if (boost::regex_match(node->getNodeGUID(), searchStringResults, searchString)){
                                node_guids_ret = node_guids.insert(node->getNodeGUID());
                                if (node_guids_ret.second==true) node->print();
                            }
                        } catch (boost::regex_error& e) {
                            std::cerr << "Exception parsing regexp: " << e.what() << "\n";
                            return false;
                        }
                    break;
                }
            } else {
                //TODO: Handle Link attributes
            }
        }
    }
    file.close();
    return true;
}
